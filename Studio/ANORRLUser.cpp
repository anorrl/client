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
#include "V8Xml/WebParser.h"

ANORRLUser& ANORRLUser::singleton()
{
    static ANORRLUser user;
	return user;
}

ANORRLUser::ANORRLUser()
	: m_webKitUserId(-1)
	, m_userName("")
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
	try
	{
		shared_ptr<const ARL::Reflection::ValueTable> v(new ARL::Reflection::ValueTable);
		ARL::WebParser::parseJSONTable(future.get(), v);
		m_webKitUserId = v->at("UserId").cast<int>();
		m_userName = v->at("Name").cast<std::string>();
	}
	catch (...)
	{
		m_webKitUserId = 0;
		m_userName = "";
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

std::string ANORRLUser::getUserName() {
	if (m_userName == "")
	{
		// make sure we are authenticated before querying user id		
		AuthenticationHelper::Instance().waitForHttpAuthentication();
		if (!m_webKitUserIDQuery.valid())
			getWebkitUserId();

		m_webKitUserIDQuery.get();
	}

	return m_userName;
}