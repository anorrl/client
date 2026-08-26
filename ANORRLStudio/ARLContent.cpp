/**
 * RbxContent.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ARLContent.h"

// ANORRL Headers
#include "util/Http.h"
#include "util/standardout.h"

#include "ANORRLGameExplorer.h"
#include "UpdateUIManager.h"

FASTFLAG(StudioEnableGameAnimationsTab)

ARLContent::ARLContent( QObject *parent)
: QObject(parent)
{
}

// Publish Selection To ANORRL
QString ARLContent::Upload(const QString &urlQStr)
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
		ANORRLGameExplorer& gameExplorer = UpdateUIManager::Instance().getViewWidget<ANORRLGameExplorer>(eDW_GAME_EXPLORER);		
		QMetaObject::invokeMethod(&gameExplorer, "reloadDataFromWeb", Qt::QueuedConnection);
	}

	return QString::fromStdString(response);
}


