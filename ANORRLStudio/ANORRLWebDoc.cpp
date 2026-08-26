/**
 * ANORRLWebDoc.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLWebDoc.h"

// Qt Headers
#include <QToolBar>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDesktopServices>
#include <QGridLayout>
#include <QNetworkReply>
#include <QWebChannel>
#include <QWebEnginePage>
#include <QWebEngineScript>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineScriptCollection>
#include <QDebug>
#include <QFile>
#include <QSslError>

// ANORRL Headers
#include "util/Statistics.h"
#include "util/Http.h"
#include "v8datamodel/FastLogSettings.h"

// ANORRL Studio Headers
#include "ARLWorkspace.h"
#include "ANORRLBrowser.h"
#include "ANORRLMainWindow.h"
#include "ANORRLWebPage.h"
#include "UpdateUIManager.h"
#include "QtUtilities.h"
#include "AuthenticationHelper.h"
#include "ANORRLCookieJar.h"
#include "ANORRLDocManager.h"

FASTFLAG(StudioInSyncWebKitAuthentication)
FASTFLAG(WebkitLocalStorageEnabled);
FASTFLAG(WebkitDeveloperToolsEnabled);
FASTFLAG(StudioEnableWebKitPlugins);
FASTFLAGVARIABLE(FunnyBrowserThing, true);

ANORRLWebDoc::ANORRLWebDoc(const QString& displayName, const QString& keyName)
: m_pWebView(NULL)
, m_pWrapperWidget(NULL)
, m_pWorkspace(new ARLWorkspace(this, NULL))
, m_pAddrInputComboBox(NULL)
, m_displayName(displayName)
, m_currentUrl("")
, m_homeUrl("")
, m_keyName(keyName)
{
}

ANORRLWebDoc::~ANORRLWebDoc()
{
	if (m_pWrapperWidget)
		m_pWrapperWidget->deleteLater();
}

bool ANORRLWebDoc::open(ANORRLMainWindow *pMainWindow, const QString& url)
{
	bool success = true;
	
	try
	{
		if(!m_pWrapperWidget)
		{
			//create parent widget
			m_pWrapperWidget = new QWidget(pMainWindow);
			m_homeUrl = url;

			//create required widgets for the web view
			setupWebView(m_pWrapperWidget);
			
			//setup layout
			QGridLayout *layout = new QGridLayout(m_pWrapperWidget);
			layout->setContentsMargins(0, 0, 0, 0);
			layout->setSpacing(0);
			
			if (ARL::ClientAppSettings::singleton().GetValueWebDocAddressBarEnabled())
			{
				QToolBar *addrToolBar = setupAddressToolBar(m_pWrapperWidget);
			    layout->addWidget(addrToolBar, 0, 0);
			}

			layout->addWidget(m_pWebView, 1, 0);
			m_pWrapperWidget->setLayout(layout);
		}

		navigateUrl(url);
	}
	
	catch(...)
	{
		success =  false;
	}
	
	return success;
}

bool ANORRLWebDoc::doClose()
{
	return true;
}

QWidget* ANORRLWebDoc::getViewer() 
{ 
	return m_pWrapperWidget; 
}

void ANORRLWebDoc::activate()
{
	if (m_bActive)
		return;

	//update toobars
	UpdateUIManager::Instance().updateToolBars();

	m_bActive = true;
}

bool ANORRLWebDoc::actionState(const QString &actionID, bool &enableState, bool &checkedState)
{
	static QString webActiveActions("fileCloseAction");
	if (webActiveActions.contains(actionID))
	{
		enableState = true;
	}
	else if (UpdateUIManager::Instance().getDockActionNames().contains(actionID))
	{
		enableState = true;
		checkedState = UpdateUIManager::Instance().getAction(actionID)->isChecked();
	}
	else if (actionID == "axisWidgetAction" || actionID == "toggleGridAction")
	{
		enableState = false;
		checkedState = UpdateUIManager::Instance().getAction(actionID)->isChecked();
	}
	else
	{
		enableState = false;
		checkedState = false;
	}

	return true;	
}

void ANORRLWebDoc::setupWebView(QWidget *wrapperWidget)
{
	m_pWebView = new ANORRLBrowser(wrapperWidget);
	m_pWebView->setPage(new ANORRLWebPage(wrapperWidget));

	// Reload page if authentication changes, required for webpages other than start page
	if (FFlag::StudioInSyncWebKitAuthentication && m_keyName != "StartPage")
		connect(&AuthenticationHelper::Instance(), SIGNAL(authenticationChanged(bool)), this, SLOT(onAuthenticationChanged(bool)));
	
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
	
	/// Keep all this for now, later on we should remove it depending on bare minimum required.
	globalSetting->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
	globalSetting->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);

	if(FFlag::WebkitLocalStorageEnabled)
		globalSetting->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);

	//initJavascript();

	// update title only for StartPage
	if (m_keyName == "StartPage")
		connect(m_pWebView, SIGNAL(titleChanged(QString)), SLOT(updateTitle(QString)));
}

void ANORRLWebDoc::handleHomeClicked()
{
	navigateUrl(m_homeUrl);
}

QToolBar* ANORRLWebDoc::setupAddressToolBar(QWidget *wrapperWidget)
{
	QToolBar *pToolBar = new QToolBar(wrapperWidget);
	
	//set the layout for the address toolbar.
	QLabel *pLabel = new QLabel(pToolBar);
	pLabel->setText(QString("Address"));
	pLabel->setMinimumSize(10, 0);

	m_pAddrInputComboBox = new QComboBox(pToolBar);
	m_pAddrInputComboBox->setEditable(true);
	m_pAddrInputComboBox->setSizePolicy(QLineEdit().sizePolicy());

	//QT has implementations for the back, forward, stop, and reload actions already.
	QAction* pAction = m_pWebView->pageAction(QWebEnginePage::Back);
	pAction->setStatusTip("Go Back");
	QtUtilities::setActionShortcuts(*pAction,QKeySequence::keyBindings(QKeySequence::Back));
	pAction->setShortcutContext(Qt::WidgetShortcut);
	pToolBar->addAction(pAction);

	pAction = m_pWebView->pageAction(QWebEnginePage::Forward);
	pAction->setStatusTip("Go Forward");
	QtUtilities::setActionShortcuts(*pAction,QKeySequence::keyBindings(QKeySequence::Forward));
	pAction->setShortcutContext(Qt::WidgetShortcut);
	pToolBar->addAction(pAction);

	pAction = m_pWebView->pageAction(QWebEnginePage::Stop);
	pAction->setStatusTip("Stop");
	pToolBar->addAction(pAction);

	pAction = m_pWebView->pageAction(QWebEnginePage::Reload);
	pAction->setStatusTip("Reload");
    QtUtilities::setActionShortcuts(*pAction,QKeySequence::keyBindings(QKeySequence::Refresh));
	pAction->setShortcutContext(Qt::WidgetShortcut);
	pToolBar->addAction(pAction);

	pAction = new QAction("Home", pToolBar);
	pAction->setIcon(QIcon(":/images/home_button.png"));
	pAction->setToolTip("Home");
	pAction->setStatusTip("Home");
	pAction->setObjectName("actionNavigateHome");

    QList<QKeySequence> sequences;
#ifdef Q_OS_WIN32
    sequences.append(QKeySequence("Alt+Home"));
#else
    sequences.append(QKeySequence("Ctrl+Home"));
#endif
    QtUtilities::setActionShortcuts(*pAction,sequences);
	pAction->setShortcutContext(Qt::WidgetShortcut);
	pToolBar->addAction(pAction);
	connect(pAction, SIGNAL(triggered()), this, SLOT(handleHomeClicked()));

	pToolBar->addWidget(pLabel);
	pToolBar->addWidget(m_pAddrInputComboBox);
	
	pToolBar->setIconSize(QSize(16, 16));

	//setup the connection for address toolbar. (home/ comboBox edit line)
	connect(m_pAddrInputComboBox, SIGNAL(activated(const QString& )), this, SLOT(navigateUrl(const QString& )));
	connect(m_pWebView, SIGNAL(urlChanged(const QUrl&)), this, SLOT(updateAddressBar(const QUrl&)));
	
	return pToolBar;
}

void ANORRLWebDoc::navigateUrl(const QString& urlString)
{
	if (urlString.isEmpty()) 
		return;
	
	QString urlStringMod(urlString);

	//append http if it's not there already
	if (!urlStringMod.contains("://"))
		urlStringMod.prepend("http://");

	QByteArray ba = urlStringMod.toLatin1();
	const char *c_str = ba.data();

	if (!FFlag::FunnyBrowserThing) {
		if (ARL::Http::trustCheckBrowser(c_str))
		{
			m_pWebView->load(urlStringMod);
		}
		else
		{
			QDesktopServices::openUrl(urlStringMod);
			urlStringMod = "";
		}
	}
	else {
		m_pWebView->load(urlStringMod);
	}
	

	updateAddressBar(QUrl(urlStringMod));
}

void ANORRLWebDoc::updateAddressBar(const QUrl& url)
{ 
	QString urlStr = url.toString();
	if (m_currentUrl == urlStr)
		return;

	m_currentUrl = urlStr;

	if (m_pAddrInputComboBox)
		m_pAddrInputComboBox->setEditText(m_currentUrl);
}

void ANORRLWebDoc::initJavascript()
{
	if (m_pWebView->page())
	{
		if (!m_pWebView->page()->webChannel())
		{
			QWebChannel* channel = new QWebChannel(m_pWebView->page());
			m_pWebView->page()->setWebChannel(channel);
		}

		QFile webChannelJsFile(":/qtwebchannel/qwebchannel.js");
		if (!webChannelJsFile.open(QIODevice::ReadOnly)) {
			qDebug() << QString("Couldn't open qwebchannel.js file: %1").arg(webChannelJsFile.errorString());
			ARL::StandardOut::singleton()->printf(ARL::MESSAGE_INFO, "CANNOT OPEN THE DAMN THING %s", QString("%1").arg(webChannelJsFile.errorString()).toStdString().c_str());
		}

		// remove all slots connected to workspace before adding it again
		// or else we can have multiple slots getting called from web page, resulting in dangling function calls.
		m_pWorkspace->disconnect();
		m_pWebView->page()->webChannel()->registerObject(QString("ARLext"), m_pWorkspace.get());
		m_pWebView->page()->runJavaScript(
			QStringLiteral("new QWebChannel(qt.webChannelTransport, function(channel) { window.external = channel.objects.ARLext; });"));
	}
}

void ANORRLWebDoc::refreshPage()
{
	if (m_pWebView)
	{
		m_pWebView->reload();
	}
}

void ANORRLWebDoc::sslErrorHandler(QNetworkReply* qnr, const QList<QSslError> & errlist)
{
#ifdef _DEBUG
  qDebug() << "---ANORRLWebDoc::sslErrorHandler: ";
  // show list of all ssl errors
  Q_FOREACH (QSslError err,errlist)
    qDebug() << "ssl error: " << err;
#endif
 
   qnr->ignoreSslErrors();
}

void ANORRLWebDoc::onAuthenticationChanged(bool)
{
	//make sure reload action is enabled (to avoid circular loop)
	QAction* pReloadAction = m_pWebView->page()->action(QWebEnginePage::Reload);
	if (pReloadAction && pReloadAction->isEnabled())
		QTimer::singleShot(0, pReloadAction, SLOT(trigger()));
}

void ANORRLWebDoc::updateTitle(QString title)
{
	if (!title.isEmpty())
	{
		const int maxTitleWidth = 130;
		m_displayName = title;
		ANORRLDocManager::Instance().setDocTitle(*this, qApp->fontMetrics().elidedText(title, Qt::ElideRight, maxTitleWidth), title);
	}
}