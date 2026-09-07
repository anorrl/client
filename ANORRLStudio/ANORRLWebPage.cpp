/**
 * ANORRLWebPage.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLWebPage.h"

// Qt Headers
#include <QDesktopServices>
#include <QNetworkReply>
#include <QWebFrame>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QWebHitTestResult>

// ANORRL Headers
#include "AuthenticationHelper.h"
#include "FastLog.h"
#include "ANORRLBrowser.h"
#include "ANORRLNetworkAccessManager.h"
#include "ANORRLSettings.h"
#include "QtUtilities.h"

#include <QWebElement>

QString ANORRLWebPage::userAgentForUrl(const QUrl &url) const 
{	
	return ANORRLNetworkAccessManager::Instance().getUserAgent();
}

ANORRLWebPage::ANORRLWebPage(QWidget* parent) 
    : QWebPage(parent)
{
	setNetworkAccessManager(&ANORRLNetworkAccessManager::Instance());
	connect(&ANORRLNetworkAccessManager::Instance(), SIGNAL(finished(QNetworkReply*)), this, SLOT(handleFinished(QNetworkReply*)));
}

void ANORRLWebPage::handleFinished(QNetworkReply* reply)
{
	QString status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
	if (status.toInt() != 200 || !reply->hasRawHeader("refresh"))
		return;

	QString acceptHeader = reply->request().rawHeader("accept").data();
	if (!acceptHeader.startsWith("application"))
		return;

	QString url = QString(reply->rawHeader("refresh").data());
	this->mainFrame()->load(url.mid(url.indexOf("=")+1));
}

bool ANORRLWebPage::acceptNavigationRequest ( QWebFrame * frame, const QNetworkRequest & request, NavigationType type )
{
	if (!frame) // This is a new window request, open in default browser
	{
		QDesktopServices::openUrl(request.url());
		return true;
	}
	else
		return QWebPage::acceptNavigationRequest(frame, request, type);
}

QString ANORRLWebPage::getDefaultUserAgent() const
{
	return QWebPage::userAgentForUrl(QUrl(""));
}

// If we have an override for the file upload element on this page, use that instead 
// This allows us to inject files into file input tags <input type="file" /> on the page
QString ANORRLWebPage::chooseFile( QWebFrame *originatingFrame, const QString& oldFile )
{
	if (m_overideUploadFile.isEmpty())
		return QWebPage::chooseFile(originatingFrame, oldFile);
	else
		return m_overideUploadFile;
}

void ANORRLWebPage::setUploadFile(QString selector, QString fileName)
{
	// Store for uploading
	m_overideUploadFile = fileName; 
	
	// Invoke an "file selection" behind the scenes.  This will automatically put the file into
	// the pages file input tag.
	QWebElement button = this->mainFrame()->findFirstElement(selector);
	button.setFocus();
	QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
	QCoreApplication::postEvent (this->view(), event);
}

void ANORRLWebPage::triggerAction(QWebPage::WebAction action, bool checked)
{
	if ((action == QWebPage::OpenLinkInNewWindow) && mainFrame() && !m_contextPos.isNull())
	{
		QWebHitTestResult hitTestResult = mainFrame()->hitTestContent(m_contextPos);
		if (!hitTestResult.isNull())
		{
			QUrl url = hitTestResult.linkUrl();
			if (url.isValid() && !url.host().isEmpty())
				QDesktopServices::openUrl(url);
		}
		m_contextPos = QPoint();
	}
	else
	{
		QWebPage::triggerAction(action, checked);
	}
}

bool ANORRLWebPage::event(QEvent *evt)
{
	if (evt->type() == QEvent::ContextMenu)
       m_contextPos = static_cast<QContextMenuEvent*>(evt)->pos();

	return QWebPage::event(evt);
}