/**
 * ANORRLUser.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLUser.h"

// ANORRL headers
#include "ARL/debug.h"

// ANORRL Studio headers
#include "ANORRLCookieJar.h"
#include "ANORRLNetworkAccessManager.h"
#include "ANORRLNetworkReply.h"
#include "ANORRLSettings.h"
#include "AuthenticationHelper.h"

#include "V8DataModel/Stats.h"

ANORRLUser& ANORRLUser::singleton()
{
    static ANORRLUser user;
	return user;
}

ANORRLUser::ANORRLUser()
	: m_webKitUserId(-1)
{
    connect(&AuthenticationHelper::Instance(), SIGNAL(authenticationChanged(bool)), this, SLOT(onAuthenticationChanged(bool)));
}

ANORRLUser::~ANORRLUser()
{
	m_webKitUserIDQuery.get();
}

void ANORRLUser::init()
{
	if (m_webKitUserId == -1)
		getWebkitUserId();
}

// Keep this user up to sync on auth changes
void ANORRLUser::onAuthenticationChanged(bool)
{
	m_webKitUserId = -1;
	getWebkitUserId();
}

void ANORRLUser::currentUserReplied(ARL::HttpFuture future)
{
	try {
		QString webKitUserIDStr(QString::fromStdString(future.get()));
		m_webKitUserId = webKitUserIDStr.toInt();
		ARL::Analytics::setUserId(m_webKitUserId);
	}
	catch(std::exception&)
	{
		m_webKitUserId = 0;
	}
}

void ANORRLUser::getWebkitUserId()
{
	std::string url(AuthenticationHelper::getLoggedInUserUrl().toStdString());
    m_webKitUserIDQuery = ARL::HttpAsync::get(url).then(boost::bind(&ANORRLUser::currentUserReplied, this, _1));
}

int ANORRLUser::getUserId()
{
	if(m_webKitUserId == -1)
	{
		// make sure we are authenticated before querying user id		
		AuthenticationHelper::Instance().waitForHttpAuthentication();
		if (!m_webKitUserIDQuery.valid())
			getWebkitUserId();		

		m_webKitUserIDQuery.get();
	}

	return m_webKitUserId;
}
