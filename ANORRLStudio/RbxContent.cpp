/**
 * RbxContent.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "RbxContent.h"

// Roblox Headers
#include "util/Http.h"
#include "util/standardout.h"

#include "RobloxGameExplorer.h"
#include "UpdateUIManager.h"

FASTFLAG(StudioEnableGameAnimationsTab)

RbxContent::RbxContent( QObject *parent) 
: QObject(parent)
{
}

// Publish Selection To Roblox
QString RbxContent::Upload(const QString &urlQStr)
{
	QByteArray ba = urlQStr.toLocal8Bit();
	const char *url = ba.data();
	
	bool success = ARL::Http::trustCheck(url);
	std::string response;
	if(success)
	{
		try 
		{			
			ARL::Http http(url);
			http.post(data, ARL::Http::kContentTypeApplicationXml, true, response);
		}
		catch (std::exception &e) 
		{
			ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR, e.what());
			return QString("");
		}
	}

	// if we are creating 'new' animation game asset then we will need to reload game explorer 
	if (FFlag::StudioEnableGameAnimationsTab && !response.empty() && 
		urlQStr.contains("uploadnewanimation?assetTypeName=Animation") && urlQStr.contains("isGamesAsset=True"))
	{
		RobloxGameExplorer& gameExplorer = UpdateUIManager::Instance().getViewWidget<RobloxGameExplorer>(eDW_GAME_EXPLORER);		
		QMetaObject::invokeMethod(&gameExplorer, "reloadDataFromWeb", Qt::QueuedConnection);
	}

	return QString::fromStdString(response);
}


