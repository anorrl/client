/**
 * WebDialog.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "WebDialog.h"

// Qt Headers
#include <QWebEngineSettings>
#include <QCloseEvent>
#include <QWebEngineProfile>
#include <QWebEngineScriptCollection>
#include <QWebChannel>
#include <QFile>

// ANORRL Headers
#include "util/Statistics.h"

// ANORRL Studio Headers
#include "ARLWorkspace.h"
#include "ANORRLBrowser.h"
#include "ANORRLSettings.h"
#include "ANORRLWebPage.h"
#include "QtUtilities.h"
#include "AuthenticationHelper.h"

FASTFLAG(WebkitLocalStorageEnabled);
FASTFLAG(WebkitDeveloperToolsEnabled);
FASTFLAG(StudioEnableWebKitPlugins);

WebDialog::WebDialog(QWidget* parent, const QString& initialUrl, ARL::DataModel *dm, int widthInPixels, int heightInPixels)
: ANORRLSavingStateDialog<QDialog>(parent, "WebDialog/Geometry")
, m_initialUrl(initialUrl)
, m_pWebView(NULL)
, m_pDataModel(dm)
, m_iWidthInPixels(widthInPixels)
, m_iHeightInPixels(heightInPixels)
{
	// Webview
	m_pWebView = new ANORRLBrowser(this);
	m_pWebPage = new ANORRLWebPage(m_pWebView);
	m_pWebView->setPage(m_pWebPage);
		
	load(m_initialUrl);

	QWebEngineSettings *globalSetting = QWebEngineSettings::globalSettings();
		
	globalSetting->setAttribute(QWebEngineSettings::AutoLoadImages, true);
	
	globalSetting->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
	globalSetting->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
	globalSetting->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
	
#ifdef _WIN32
    if (FFlag::StudioEnableWebKitPlugins)
        globalSetting->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    else
        globalSetting->setAttribute(QWebEngineSettings::PluginsEnabled, false);
#endif

	if(FFlag::WebkitLocalStorageEnabled)
		globalSetting->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);

	m_pWorkspace.reset(new ARLWorkspace(this, m_pDataModel));
	connect(m_pWorkspace.get(), SIGNAL(closeEvent()), this, SLOT(close()));
	connect(m_pWorkspace.get(), SIGNAL(hideEvent()), this, SLOT(hide()));
	connect(m_pWorkspace.get(), SIGNAL(showEvent()), this, SLOT(show()));

	connect(m_pWebView->page(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(initJavascript()));  
	connect(m_pWebPage, SIGNAL(windowCloseRequested()), this, SLOT(close()));
	
	// Do not show the help button
	Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
	flags = flags & (~helpFlag);
	setWindowFlags(flags);

	// Allow expanding and shrinking in both x and y
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	QtUtilities::updateWindowTitle(this);
}

void WebDialog::load(const QString& url)
{
	m_pWebView->load(url);	
}

void WebDialog::initJavascript()
{
	m_pWorkspace->disconnect();
	/*QWebChannel* channel = new QWebChannel();
	m_pWebView->page()->setWebChannel(channel);
	channel->registerObject(QString("anorrl"), m_pWorkspace.get());

	QFile webChannelFile(":/qtwebchannel/qwebchannel.js");
	if (!webChannelFile.open(QIODevice::ReadOnly))
	{
		qWarning() << "Failed to open qwebchannel.js";
		return;
	}
	QString jsStr = QString::fromUtf8(webChannelFile.readAll());
	webChannelFile.close();

	QWebEngineScript webChannelScript;
	webChannelScript.setName("qwebchannel.js");
	webChannelScript.setInjectionPoint(QWebEngineScript::DocumentCreation);
	webChannelScript.setRunsOnSubFrames(false);
	webChannelScript.setWorldId(QWebEngineScript::MainWorld);
	webChannelScript.setSourceCode(jsStr);
	m_pWebView->page()->scripts().insert(webChannelScript);*/
}

void WebDialog::resizeEvent( QResizeEvent* evt )
{
	QDialog::resizeEvent(evt);
	
	m_pWebView->resize(width(), height());
}

QSize WebDialog::sizeHint() const
{
	return QSize(m_iWidthInPixels, m_iHeightInPixels);
}

UploadDialog::UploadDialog(QWidget* parent, ARL::DataModel* dataModel)
: WebDialog(parent, QString("%1/Game/Upload.aspx").arg(ANORRLSettings::getBaseURL()), dataModel)
{
	setWindowModality(Qt::WindowModal);
}

void UploadDialog::reject()
{    
    QDialog::reject();
    setResult(CANCEL);
}

void UploadDialog::closeEvent(QCloseEvent* evt)
{
    setResult(CANCEL);
	evt->accept();
}




