/**
* ANORRLWebPage .h
* Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
*/

#pragma once

#include <QWebEnginePage>
#include <QWebEngineCertificateError>
#include <QNetworkReply>
#include <QPoint>

class ANORRLWebPage : public QWebEnginePage
{
	Q_OBJECT

public:
	ANORRLWebPage(QWidget* parent = 0);
	QString userAgentForUrl(const QUrl &url) const;
	static QWebEngineProfile* getSharedProfile();

protected:
	virtual bool event(QEvent *evt);
	void onCertificateError(QWebEngineCertificateError error);

private:
	QString m_overideUploadFile;
	QPoint  m_contextPos;
};