/**
 * StudioSerializerHelper.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "StudioSerializerHelper.h"

// Qt Headers
#include <QFileInfo>
#include <QTemporaryFile>
#include <QDir>
#include <QCryptographicHash>

// Roblox Headers
#include "v8datamodel/DataModel.h"
#include "V8DataModel/PartOperationAsset.h"
#include "v8xml/XmlSerializer.h"
#include "v8xml/SerializerBinary.h"
#include "script/DebuggerManager.h"
#include "util/MD5Hasher.h"

// Roblox Studio Headers
#include "UpdateUIManager.h"
#include "QtUtilities.h"
#include "StudioUtilities.h"
#include "RobloxMainWindow.h"

FASTFLAG(LuaDebugger)
FASTFLAG(StudioCSGAssets)

QMutex StudioSerializerHelper::m_Mutex;

bool StudioSerializerHelper::saveAs(
    const QString&  fileName,
    const QString&  label,
    bool            asynchronous,
    bool            useBinaryFormat,
    ARL::DataModel* pDataModel,
    QString&        outErrorMessage,
    bool            publishAssets)
{
    bool result = false;
    outErrorMessage.clear();

    if ( fileName.isEmpty() )
    {
        outErrorMessage = "Invalid filename for saving.";
        ARL::StandardOut::singleton()->print(
            ARL::MESSAGE_ERROR,
            qPrintable(tr("Error saving file '%1' - %2").arg(fileName).arg(outErrorMessage)) );
        return false;
    }

    // append file extension if it's not there already!
	QString outputFilename(fileName);
	if ( !outputFilename.endsWith(".rbxl", Qt::CaseInsensitive) && !outputFilename.endsWith(".rbxlx", Qt::CaseInsensitive) )
		outputFilename.append(".rbxl");
	
    // make sure the file can be written to
    QFileInfo outputFileInfo(outputFilename);
	if ( outputFileInfo.exists() && !outputFileInfo.isWritable() ) 
	{
        outErrorMessage = tr(
            "Failed to write to file '%1'.\n\n"
            "Make sure this file is not set to read-only.").
            arg(outputFilename);
        ARL::StandardOut::singleton()->print(
            ARL::MESSAGE_ERROR,
            qPrintable(tr("Error saving file '%1' - %2").arg(fileName).arg(outErrorMessage)) );
        return false;
    }

    if (FFlag::StudioCSGAssets && publishAssets && pDataModel->getPlaceID() != 0)
    {
        try
        {
       		ARL::DataModel::LegacyLock lock(pDataModel, ARL::DataModelJob::Write);
            const int publishTimeoutMills = 240000;
            ARL::PartOperationAsset::publishAll(pDataModel, publishTimeoutMills);
        }
        catch(ARL::DataModel::SerializationException&)
        {
            // Do nothing and save unions locally.
        }
    }

    if ( useBinaryFormat )
    {
        UpdateUIManager::Instance().waitForLongProcess(
            label,
            boost::bind(&StudioSerializerHelper::serializeBinary,fileName,pDataModel,&result,&outErrorMessage) );
    }
    else
    {
        // output xml data structure
        //  will be deleted in the serialization stage
        XmlElement* xml = Serializer::newRootElement();

        // first stage - save to XML data structure
        if ( !asynchronous )
            UpdateUIManager::Instance().setBusy(true);

        UpdateUIManager::Instance().waitForLongProcess(
            label,
            boost::bind(&StudioSerializerHelper::generateXML,pDataModel,xml) );

        if ( asynchronous )
        {
            new boost::thread(
                boost::bind(&StudioSerializerHelper::serializeXMLAsynchronous,outputFilename,xml) );
            result = true;
        }
        else
        { 
            // second stage - save xml data to disk
            UpdateUIManager::Instance().waitForLongProcess(
                label,
                boost::bind(&StudioSerializerHelper::serializeXML,outputFilename,xml,&result,&outErrorMessage) );
            UpdateUIManager::Instance().setBusy(false);
        }
    }

    if ( !result )
	{
        ARL::StandardOut::singleton()->print(
            ARL::MESSAGE_ERROR,
            qPrintable(tr("Error saving file '%1' - %2").arg(fileName).arg(outErrorMessage)) );
    }

    return result;
}

/**
 * Given a datamodel, generate the XML required for saving.
 *  
 */
void StudioSerializerHelper::generateXML(
    ARL::DataModel* pDataModel,
    XmlElement*     xml )
{
    ARL::DataModel::LegacyLock lock(pDataModel, ARL::DataModelJob::Write);
    pDataModel->writeChildren(xml,ARL::SerializationCreator);
}

/**
 * Serialize XML to disk.
 *  The xml elements will be deleted after serialization.
 */
void StudioSerializerHelper::serializeXML(
    const QString&  fileName,
    XmlElement*     xml,
    bool*           outResult,
    QString*        outErrorMessage )
{
    QMutexLocker locker(&m_Mutex);

    ARLASSERT(xml);
    ARLASSERT(outResult);
    ARLASSERT(outErrorMessage);

    *outResult = false;
    *outErrorMessage = "";

    // create a temp file on disk, open to force creation
    QTemporaryFile tempFile(QDir::tempPath() + "/ARL-XXXXXX.rbxl");
    if ( !tempFile.open() )
    {
        *outErrorMessage = tr(
            "Failed to open temporary file while saving.\n\n"
            "Make sure there is enough hard drive space for the directory '%1' "
            "and please try again.").
            arg(QDir::tempPath());
        *outResult = false;
        return;
    }

	try
	{
		std::ofstream stream;
	    stream.open(qPrintable(tempFile.fileName()),std::ios_base::out | std::ios::binary);
		
        // serialize the data model
		TextXmlWriter machine(stream);
        machine.serialize(xml);

		// don't need XML any more, delete it
        delete xml;

        // force the temp file to flush and be unlocked
        stream.close();
        tempFile.close();

        // remove original file
        QFileInfo outputFileInfo(fileName);
        if ( outputFileInfo.exists() && !QFile::remove(fileName) )
        {
            *outErrorMessage = tr(
                "Failed to delete original file '%1'.\n\n"
                "Original data has not been overwritten.  "
                "Please try again or save with a different file name." ).
                arg(fileName);
            *outResult = false;
            return;
        }

        // rename temp file to original file - cross fingers that this works!
        if ( !QFile::copy(tempFile.fileName(),fileName) )
        {
            *outErrorMessage = tr(
                "Failed to save data to '%1'.\n\n"
                "Data is temporarily saved in '%2'." ).
                arg(fileName).arg(tempFile.fileName());
            tempFile.setAutoRemove(false);  // don't remove it, this is a valid copy
            *outResult = false;
            return;
		}

		if (FFlag::LuaDebugger)
			StudioSerializerHelper::saveDebuggerData(StudioUtilities::getDebugInfoFile(fileName));

        *outResult = true;
	}	
	catch (...)
	{
        *outResult = false;
		*outErrorMessage = tr(
            "Error in saving data to '%1'.\n\n"
            "Please try again or save with a different file name." ).
            arg(fileName);
	}
}

void StudioSerializerHelper::serializeXMLAsynchronous(
    const QString&  fileName,
    XmlElement*     xml )
{
    QMutexLocker locker(&m_Mutex);
    ARLASSERT(xml);

    // create a temp file on disk, open to force creation
    QTemporaryFile tempFile(QDir::tempPath() + "/ARL-XXXXXX.rbxl");
    if ( !tempFile.open() )
    {
        QString message = tr(
            "Failed to open temporary file while saving.\n\n"
            "Make sure there is enough hard drive space for the directory '%1' "
            "and please try again.").
            arg(QDir::tempPath());
        ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR,qPrintable(message));
        return;
    }

	try
	{
		std::ofstream stream;
	    stream.open(qPrintable(tempFile.fileName()),std::ios_base::out | std::ios::binary);
		
        // serialize the data model
		TextXmlWriter machine(stream);
        machine.serialize(xml);

		// don't need XML any more, delete it
        delete xml;

        // force the temp file to flush and be unlocked
        stream.close();
        tempFile.close();

        // remove original file
        QFileInfo outputFileInfo(fileName);
        if ( outputFileInfo.exists() && !QFile::remove(fileName) )
        {
            QString message = tr(
                "Failed to delete original file '%1'.\n\n"
                "Original data has not been overwritten.  "
                "Please try again or save with a different file name." ).
                arg(fileName);
            ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR,qPrintable(message));
            return;
        }

        // rename temp file to original file - cross fingers that this works!
        if ( !QFile::copy(tempFile.fileName(),fileName) )
        {
            QString message = tr(
                "Failed to save data to '%1'.\n\n"
                "Data is temporarily saved in '%2'." ).
                arg(fileName).arg(tempFile.fileName());
            tempFile.setAutoRemove(false);  // don't remove it, this is a valid copy
            ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR,qPrintable(message));
            return;
		}

		if (FFlag::LuaDebugger)
			StudioSerializerHelper::saveDebuggerData(StudioUtilities::getDebugInfoFile(fileName));
	}	
	catch (...)
	{
        QString message = tr(
            "Error in saving data to '%1'.\n\n"
            "Please try again or save with a different file name." ).
            arg(fileName);
        ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR,qPrintable(message));
	}
}

void StudioSerializerHelper::serializeBinary(
    const QString&  fileName,
    ARL::DataModel* pDataModel,
    bool*           outResult,
    QString*        outErrorMessage )
{
    QMutexLocker locker(&m_Mutex);
    ARLASSERT(pDataModel);

    *outResult = false;
    *outErrorMessage = "";

    // create a temp file on disk, open to force creation
    QTemporaryFile tempFile(QDir::tempPath() + "/ARL-XXXXXX.bin.rbxl");
    if ( !tempFile.open() )
    {
        *outErrorMessage = tr(
            "Failed to open temporary file while saving.\n\n"
            "Make sure there is enough hard drive space for the directory '%1' "
            "and please try again.").
            arg(QDir::tempPath());
        *outResult = false;
        return;
    }

	try
	{
		std::ofstream stream;
	    stream.open(qPrintable(tempFile.fileName()),std::ios_base::out | std::ios::binary);
		
        // serialize the data model
		ARL::SerializerBinary::serialize(stream,pDataModel);

        // force the temp file to flush and be unlocked
        stream.close();
        tempFile.close();

        // remove original file
        QFileInfo outputFileInfo(fileName);
        if ( outputFileInfo.exists() && !QFile::remove(fileName) )
        {
            *outErrorMessage = tr(
                "Failed to delete original file '%1'.\n\n"
                "Original data has not been overwritten.  "
                "Please try again or save with a different file name." ).
                arg(fileName);
            *outResult = false;
            return;
        }

        // rename temp file to original file - cross fingers that this works!
        if ( !QFile::copy(tempFile.fileName(),fileName) )
        {
             *outErrorMessage = tr(
                "Failed to save data to '%1'.\n\n"
                "Data is temporarily saved in '%2'." ).
                arg(fileName).arg(tempFile.fileName());
            tempFile.setAutoRemove(false);  // don't remove it, this is a valid copy
            *outResult = false;
            return;
		}

		if (FFlag::LuaDebugger)
			StudioSerializerHelper::saveDebuggerData(StudioUtilities::getDebugInfoFile(fileName));

        *outResult = true;
	}	
	catch (...)
	{
        *outResult = false;
		*outErrorMessage = tr(
            "Error in saving data to '%1'.\n\n"
            "Please try again or save with a different file name." ).
            arg(fileName);
	}
}

QByteArray StudioSerializerHelper::getDataModelHash(ARL::DataModel* pDataModel)
{
	std::stringstream strStream(std::ios_base::out | std::ios::binary);
	{
		ARL::DataModel::LegacyLock lock(pDataModel, ARL::DataModelJob::Read);
		ARL::SerializerBinary::serialize(strStream, pDataModel);
	}
	QByteArray byteArray = QByteArray(strStream.str().c_str(), strStream.str().length());

	QCryptographicHash shaHasher(QCryptographicHash::Sha1);
	shaHasher.addData(byteArray);
	QByteArray dataModelHash = shaHasher.result();
	return dataModelHash;
}

bool StudioSerializerHelper::saveAsIfModified(const QByteArray& hashToCompareWith, ARL::DataModel* pDataModel, const QString& fileName)
{
	bool result = false;

	std::stringstream strStream(std::ios_base::out | std::ios::binary);
	{
		ARL::DataModel::LegacyLock lock(pDataModel, ARL::DataModelJob::Write);
		ARL::SerializerBinary::serialize(strStream, pDataModel);
	}

	QByteArray byteArray = QByteArray(strStream.str().c_str(), strStream.str().length());
	QCryptographicHash shaHasher(QCryptographicHash::Sha1);
	shaHasher.addData(byteArray);

	QByteArray dataModelHash = shaHasher.result();
	if (dataModelHash != hashToCompareWith)
	{
		try 
		{
			std::ofstream stream;
			stream.open(qPrintable(fileName),std::ios_base::out | std::ios::binary);
			stream.write( strStream.str().c_str(), strStream.str().length() );
			stream.close();
			result = true;
		}

		catch (...)
		{
		}
	}

	return result;
}

bool StudioSerializerHelper::saveDebuggerData(const QString& debugInfoFile)
{
	bool outResult = true;
	try
	{
		ARL::Scripting::DebuggerManager& debuggerManager = ARL::Scripting::DebuggerManager::singleton();
				
		const ARL::Scripting::DebuggerManager::Debuggers debuggers = debuggerManager.getDebuggers();
		if (debuggers.size() > 0)
		{
			// first remove the existing file 
			QFile::remove(debugInfoFile);

			// serialize debugger data
			std::ofstream stream;
			stream.open(qPrintable(debugInfoFile), std::ios_base::out | std::ios::binary);

			// now serialize debugger data
			ARL::SerializerBinary::serialize(stream, &debuggerManager);

			// close stream
			stream.close();
		}
	}

	catch (...)
	{
		outResult = false;
	}

	return outResult;
}
