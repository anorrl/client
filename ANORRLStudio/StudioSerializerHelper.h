/**
 * StudioSerializerHelper.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QString>
#include <QMutex>
#include <QObject>

// Roblox Headers
#include "v8xml/Serializer.h"
#include "v8xml/XmlElement.h"

namespace ARL
{
	class DataModel;
}

/**
 * Serializes a DataDodel to disk either synchronously or asynchronously.
 *  Displays progress bars.
 */
class StudioSerializerHelper : public QObject
{
	public:

		static bool saveAs(
            const QString&  fileName,
            const QString&  label,
            bool            asynchronous,
            bool            useBinaryFormat,
            ARL::DataModel* pDataModel,
            QString&        outErrorMessage,
            bool            publishAssets);

		static bool saveDebuggerData(const QString& debuggerFileName);

		static QByteArray getDataModelHash(ARL::DataModel* pDataModel);

		static bool saveAsIfModified(const QByteArray& hashToCompareWith, ARL::DataModel* pDataModel, const QString& fileName);
	
	private:

		static void generateXML(
            ARL::DataModel* pDataModel,
            XmlElement*     xml );

        static void serializeXML(
            const QString&  fileName,
            XmlElement*     xml,
            bool*           result,
            QString*        outErrorMessage );

        static void serializeXMLAsynchronous(
            const QString&  fileName,
            XmlElement*     xml );

        static void serializeBinary(
            const QString&  fileName,
            ARL::DataModel* pDataModel,
            bool*           outResult,
            QString*        outErrorMessage );

		static QMutex m_Mutex;
};

