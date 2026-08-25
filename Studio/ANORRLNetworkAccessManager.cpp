/**
 * ANORRLNetworkAccessManager.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLNetworkAccessManager.h"

// ANORRL Headers
#include "QtUtilities.h"
#include "ANORRLCookieJar.h"
#include "ANORRLWebPage.h"
#include "ANORRLSettings.h"
#include "ANORRLNetworkReply.h"

ANORRLNetworkAccessManager::ANORRLNetworkAccessManager()
: QNetworkAccessManager()
{

	// When we enable the Cookie Persistence, logging into mail.roblox.com stops working, but works fine for www.roblox.com with persistence
	setCookieJar(new ANORRLCookieJar());
}
void ANORRLNetworkAccessManager::initUserAgent()
{
	// Set up our useragent
	userAgent = "ANORRL Studio [[BETA]] QT5";
	
	// Only on Mac webkit does not provide the OSX version, add it, will do only for Mac
	QString macOSXVersion = QtUtilities::getMacOSXVersion();
	userAgent = userAgent.replace("Mac OS X", macOSXVersion);
}
QString ANORRLNetworkAccessManager::getUserAgent()
{
	if (userAgent.isEmpty())
		initUserAgent();
	return userAgent;
}

ANORRLNetworkReply* ANORRLNetworkAccessManager::get(const QNetworkRequest& request, bool followRedirection /*= true*/ )
{
	QNetworkReply* reply = QNetworkAccessManager::get(request);
	ANORRLNetworkReply* anorrlReply = new ANORRLNetworkReply(reply, followRedirection);
	return anorrlReply;
}

ANORRLCookieJar* ANORRLNetworkAccessManager::cookieJar() const
{
	return dynamic_cast<ANORRLCookieJar*>(QNetworkAccessManager::cookieJar());
}
