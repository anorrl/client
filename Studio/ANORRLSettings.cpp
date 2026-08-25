/**
 * ANORRLSettings.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLSettings.h"

// Qt Headers
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QDesktopServices>

// ANORRL Headers
#include "v8datamodel/ContentProvider.h"
#include "v8datamodel/DebugSettings.h"
#include "script/script.h"
#include "util/Statistics.h"

// ANORRL Studio Headers
#include "ARLWorkspace.h"

#ifdef _WIN32
    #include "ANORRLStudioVersion.h"
#endif

AppSettings& AppSettings::instance()
{			
	static AppSettings singleton;
	return singleton;
}

AppSettings::AppSettings()
: m_bIsScriptAssetUploadEnabled(false)
, m_bIsAnimationAssetUploadEnabled(false)
, m_bIsImageModelAssetUploadEnabled(false)
{
	QString executableFolder = QApplication::applicationDirPath();
	QFile file(executableFolder + "/AppSettings.xml");
	if (file.open(QIODevice::ReadOnly))
	{
		QDomDocument doc("appSettings");
		if (doc.setContent(&file)) 
		{
			QDomElement docElem = doc.documentElement();
			if(docElem.tagName() == "Settings")
			{
				QDomElement content = docElem.firstChildElement("ContentFolder");
				QString dummy = content.text();
				m_contentFolder = executableFolder + "/" + dummy;

				dummy.replace("content", "BuiltInPlugins");
				m_builtInPluginsFolder = executableFolder + "/" + dummy;

				content = docElem.firstChildElement("BaseUrl");				
				m_baseURL = content.text();

				content = docElem.firstChildElement("IsScriptAssetUploadEnabled");				
				m_bIsScriptAssetUploadEnabled = content.text() == "1";
				
				content = docElem.firstChildElement("IsAnimationAssetUploadEnabled");				
				m_bIsAnimationAssetUploadEnabled = content.text() == "1";

				content = docElem.firstChildElement("IsImageModelAssetUploadEnabled");				
				m_bIsImageModelAssetUploadEnabled = content.text() == "1";

				content = docElem.firstChildElement("CrashMenu");				
				m_bShowStudioCrashMenu = content.text() == "1";
#ifdef STUDIO_ADMIN_BUILD
				content = docElem.firstChildElement("AdminKey");				
				m_adminKey = content.text();
#endif
			}
		}
		file.close();
	}

#ifdef _WIN32
	m_tempLocation = QDir::homePath() + "/AppData/Local/ANORRL";
#else
	m_tempLocation = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + "/ANORRL";
#endif
	if(!QFile::exists(m_tempLocation))
		QDir().mkpath(m_tempLocation);
}

ANORRLSettings::ANORRLSettings()
:QSettings()
{
}

void ANORRLSettings::saveAssets()
{
	ANORRLSettings settings;
	settings.setValue("BaseUrl", QString(::GetBaseURL().c_str()));
	settings.setValue("ContentFolder", QString(ARL::ContentProvider::assetFolder().c_str()));
}

void ANORRLSettings::recoverAssets()
{
	QString baseUrl = AppSettings::instance().baseURL();
	::SetBaseURL(baseUrl.toStdString());

	QString assetFolder = AppSettings::instance().contentFolder();
	QByteArray assetFolderUtf8 = assetFolder.toUtf8();

	ARL::ContentProvider::setAssetFolder(std::string(assetFolderUtf8.data(), assetFolderUtf8.size()).c_str());
}

QString ANORRLSettings::getAssetFolder()
{
	return AppSettings::instance().contentFolder();
}

QString ANORRLSettings::getBuiltInPluginsFolder()
{
	return AppSettings::instance().builtInPluginsFolder();
}

QString ANORRLSettings::getBaseURL()
{
	return AppSettings::instance().baseURL();
}

QString ANORRLSettings::getWebURL()
{
	return AppSettings::instance().webURL();
}

QString ANORRLSettings::getApiBaseURL()
{
	return QString::fromStdString(ARL::ContentProvider::getUnsecureApiBaseUrl(getBaseURL().toStdString()));
}

#ifdef STUDIO_ADMIN_BUILD
QString ANORRLSettings::getAdminKey()
{
	return AppSettings::instance().adminKey();
}
#endif

QString ANORRLSettings::getVersionString()
{
	QString version;
#ifdef _WIN32
	version = VER_FILEVERSION_STR;
	version.replace(',','.');
#else
	QString plistPath = QApplication::applicationDirPath() + "/../info.plist";
	QSettings settings(plistPath, QSettings::NativeFormat);
	version = settings.value("CFBundleShortVersionString").toString();
#endif
	ARL::DebugSettings::anorrlVersion = version.toStdString();
	return version;

}

QString ANORRLSettings::getTempLocation()
{
	return AppSettings::instance().tempLocation();
}

QString ANORRLSettings::getResourcesFolder()
{
	QString resourcesFolder = QApplication::applicationDirPath();
#ifdef Q_OS_MAC
	resourcesFolder += "/../Resources";
#endif
	return resourcesFolder;
}

void ANORRLSettings::initWorkspaceSettings()
{
	ARLWorkspace::isScriptAssetUploadEnabled = AppSettings::instance().isScriptAssetUploadEnabled();
	ARLWorkspace::isAnimationAssetUploadEnabled = AppSettings::instance().isAnimationAssetUploadEnabled();
	ARLWorkspace::isImageModelAssetUploadEnabled = AppSettings::instance().isImageModelAssetUploadEnabled();
	
}

bool ANORRLSettings::showCrashMenu()
{
	return AppSettings::instance().showCrashMenu();
}
