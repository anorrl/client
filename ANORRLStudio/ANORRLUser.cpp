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
#include "ANORRLMainWindow.h"
#include "UpdateUIManager.h"

#include "V8DataModel/Stats.h"
#include "V8Xml/WebParser.h"

ANORRLUser& ANORRLUser::singleton()
{
    static ANORRLUser user;
	return user;
}

ANORRLUser::ANORRLUser()
	: m_webKitUserId(-1)
	, m_webKitUserName("")
{
    connect(&AuthenticationHelper::Instance(), SIGNAL(authenticationChanged(bool)), this, SLOT(onAuthenticationChanged(bool)));
}

ANORRLUser::~ANORRLUser()
{
	m_webKitUserInfoQuery.get();
}

void ANORRLUser::init()
{
	if (m_webKitUserId == -1)
		getWebkitUserInfo();
	if (m_webKitUserName.length() == 0)
		getWebkitUserInfo();
}

// Keep this user up to sync on auth changes
void ANORRLUser::onAuthenticationChanged(bool)
{
	m_webKitUserId = -1;
	m_webKitUserName = "";
	getWebkitUserInfo();
}

void ANORRLUser::currentUserReplied(ARL::HttpFuture future)
{
	try
	{
		std::string result = future.get();
		shared_ptr<const ARL::Reflection::ValueTable> v(new ARL::Reflection::ValueTable);
		ARL::WebParser::parseJSONTable(result, v);
		m_webKitUserId   = v->at("UserId").cast<int>();
		m_webKitUserName = v->at("Name").cast<std::string>();
	}
	catch (...)
	{
		m_webKitUserId = -1;
		m_webKitUserName = "";
	}

	// stupid fucking hacking because studio is a FAGGOT.
	ANORRLMainWindow& mainWindow = UpdateUIManager::Instance().getMainWindow();
	mainWindow.updateUser(m_webKitUserName);
}

void ANORRLUser::getWebkitUserInfo()
{
	std::string url(AuthenticationHelper::getLoggedInUserUrl().toStdString());
    m_webKitUserInfoQuery = ARL::HttpAsync::get(url).then(boost::bind(&ANORRLUser::currentUserReplied, this, _1));
}

int ANORRLUser::getUserId()
{
	if(m_webKitUserId == -1)
	{
		// make sure we are authenticated before querying user id		
		AuthenticationHelper::Instance().waitForHttpAuthentication();
		if (!m_webKitUserInfoQuery.valid())
			getWebkitUserInfo();		

		m_webKitUserInfoQuery.get();
	}

	return m_webKitUserId;
}

std::string ANORRLUser::getUserName() {
	if (m_webKitUserName.length() == 0 || m_webKitUserName == "")
	{
		// make sure we are authenticated before querying user id		
		AuthenticationHelper::Instance().waitForHttpAuthentication();
		if (!m_webKitUserInfoQuery.valid())
			getWebkitUserInfo();		

		m_webKitUserInfoQuery.get();
	}

	return m_webKitUserName;
}