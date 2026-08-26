/**
* ANORRLWebPage.cpp
* Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
*/

#include "stdafx.h"
#include "ANORRLWebPage.h"

// Qt Headers
#include <QDesktopServices>
#include <QNetworkReply>
#include <QWebEnginePage>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QWebEngineSettings>
#include <QWebEngineCookieStore>
#include <qwebengineprofile.h>

// Roblox Headers
#include "AuthenticationHelper.h"
#include "FastLog.h"
#include "ANORRLBrowser.h"
#include "ANORRLNetworkAccessManager.h"
#include "ANORRLSettings.h"
#include "ANORRLCookieJar.h"
#include "QtUtilities.h"

QString ANORRLWebPage::userAgentForUrl(const QUrl &url) const
{
	return "ANORRL Studio [[BETA]] QT5";
}

QWebEngineProfile* ANORRLWebPage::getSharedProfile()
{
	static QWebEngineProfile* sharedProfile = nullptr;
	if (!sharedProfile)
	{
		sharedProfile = QWebEngineProfile::defaultProfile();
		sharedProfile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);


		auto baseURL = ANORRLSettings::getWebURL();
		auto authCookie = ANORRLNetworkAccessManager::Instance().cookieJar()->getCookieValue(baseURL, "ANORRLSECURITY");
		if (!authCookie.isEmpty())
		{
			QNetworkCookie cookie;
			cookie.setName("ANORRLSECURITY");
			cookie.setValue(authCookie.toUtf8());
			cookie.setDomain(baseURL);
			cookie.setPath("/");
			cookie.setExpirationDate(QDateTime::currentDateTimeUtc().addYears(10));
			sharedProfile->cookieStore()->setCookie(cookie, baseURL);
			ARL::StandardOut::singleton()->print(ARL::MESSAGE_INFO, "FOUND AND LOADED");
		}

		QWebEngineSettings* globalSetting = sharedProfile->settings();
		globalSetting->setAttribute(QWebEngineSettings::AutoLoadImages, true);

		globalSetting->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
		globalSetting->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
		globalSetting->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);

		globalSetting->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
		globalSetting->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
	}
	return sharedProfile;
}

ANORRLWebPage::ANORRLWebPage(QWidget* parent)
	: QWebEnginePage(getSharedProfile(), parent)
{
	profile()->setHttpUserAgent(userAgentForUrl(QUrl()));
	//connect(this, &ANORRLWebPage::certificateError, this, &ANORRLWebPage::onCertificateError);
}

bool ANORRLWebPage::event(QEvent *evt)
{
	if (evt->type() == QEvent::ContextMenu)
		m_contextPos = static_cast<QContextMenuEvent*>(evt)->pos();

	return QWebEnginePage::event(evt);
}

void ANORRLWebPage::onCertificateError(QWebEngineCertificateError error) {
#ifdef DEBUG 
	qWarning() << "Ignoring certificate error: " << error.errorDescription();
	auto mutableError = const_cast<QWebEngineCertificateError&>(error);
	mutableError.acceptCertificate();
#else
	qCritical() << "Certificate error: " << error.errorDescription();
#endif
}