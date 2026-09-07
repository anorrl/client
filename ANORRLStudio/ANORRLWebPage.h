/**
 * ANORRLWebPage.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

#include <QWebPage>

class ANORRLWebPage : public QWebPage 
{
	Q_OBJECT

public: 

	ANORRLWebPage(QWidget* parent = 0);
	QString userAgentForUrl(const QUrl &url ) const;

	QString getDefaultUserAgent() const; // To get access to protected default user agent
	void setUploadFile(QString selector, QString fileName);

	virtual void triggerAction(QWebPage::WebAction action, bool checked = false);
protected:
	virtual QString chooseFile(QWebFrame *originatingFrame, const QString& oldFile);
	virtual bool acceptNavigationRequest ( QWebFrame * frame, const QNetworkRequest & request, NavigationType type );
	virtual bool event(QEvent *evt);

private Q_SLOTS:

	void handleFinished(QNetworkReply*);
private:
	QString m_overideUploadFile;
	QPoint  m_contextPos;
};