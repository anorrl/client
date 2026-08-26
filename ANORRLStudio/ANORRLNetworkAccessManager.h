/**
 * ANORRLNetworkAccessManager.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QNetworkAccessManager>

class ANORRLNetworkReply;
class ANORRLCookieJar;

class ANORRLNetworkAccessManager : public QNetworkAccessManager
{
public:
	static ANORRLNetworkAccessManager& Instance()
	{
		// TODO: ANORRLMainWindow should be the parent of this, but it's not a singleton or QObject
		// We instead set the parent during initialization.  Would be better to pass into the constructor
		static ANORRLNetworkAccessManager singleInstance;
		return singleInstance;
	}

	QString getUserAgent();
	ANORRLNetworkReply* get(const QNetworkRequest& request, bool followRedirection = true);
	ANORRLCookieJar* cookieJar() const;

private:
	ANORRLNetworkAccessManager();
	void initUserAgent();

	QString userAgent;
};