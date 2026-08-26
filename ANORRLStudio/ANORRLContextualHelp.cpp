/**
 * ANORRLContextualHelp.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"

#include "ANORRLContextualHelp.h"

#include <QGridLayout>
#include <QString>
#include <QPainter>
#include <QTimer>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QWebEngineProfile>

#include "util/standardout.h"

#include "AuthenticationHelper.h"
#include "ANORRLCookieJar.h"
#include "ANORRLNetworkAccessManager.h"
#include "ANORRLSettings.h"

FASTFLAG(WebkitLocalStorageEnabled)
FASTFLAG(WebkitDeveloperToolsEnabled)

FASTFLAGVARIABLE(StudioNewWiki, false)
FASTFLAGVARIABLE(StudioEnableWebKitPlugins, false)

ANORRLContextualHelpService::ANORRLContextualHelpService()
    : m_helpTopic("Studio")
{
}

ANORRLContextualHelpService& ANORRLContextualHelpService::singleton()
{
    static ANORRLContextualHelpService* helpService = new ANORRLContextualHelpService();
    return *helpService;
}

void ANORRLContextualHelpService::onHelpTopicChanged(const QString& helpTopic)
{
    if (m_helpTopic != helpTopic)
    {
        m_helpTopic = helpTopic;
        Q_EMIT(helpTopicChanged(helpTopic));
    }
}

ANORRLContextualHelp::ANORRLContextualHelp()
    : m_pWebView(NULL)
    , m_urlDirty(false)
{
	QGridLayout *layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	
	setupWebView();
	layout->addWidget(m_pWebView, 0, 0);
	setLayout(dynamic_cast<QLayout*>(layout));
    
    connect(&ANORRLContextualHelpService::singleton(), SIGNAL(helpTopicChanged(const QString&)), this, SLOT(onHelpTopicChanged(const QString&)));
}

void ANORRLContextualHelp::setupWebView()
{
	m_pWebView = new ANORRLHelpWebView(this);
	m_pWebPage = new ANORRLWebPage(this);
	
	m_pWebView->setPage(m_pWebPage);
    connect(&AuthenticationHelper::Instance(), SIGNAL(authenticationChanged(bool)), this, SLOT(onAuthenticationChanged(bool)));
    
	if(FFlag::StudioNewWiki)
	{
		connect(m_pWebView->page(), SIGNAL(linkClicked(const QUrl&)), this, SLOT(linkClicked(const QUrl&)));
	}
	m_urlString = QString("http://wiki.anorrl.com/index.php/StudioJARONA");

	if (FFlag::StudioNewWiki)
	{
		m_urlString += "?studiomode=true";
	}
    
    m_urlDirty = true;
    
    setEnabled(true);
	update();
}

void ANORRLContextualHelp::linkClicked(const QUrl& url)
{
	QUrl studioModeUrl = url;
	//studioModeUrl.addQueryItem("studiomode", "true");
	m_pWebView->load(studioModeUrl);
}

void ANORRLContextualHelp::onAuthenticationChanged(bool)
{
	m_pWebPage->triggerAction(QWebEnginePage::Reload);
}

void ANORRLContextualHelp::onHelpTopicChanged(const QString& helpTopic)
{
	QString prevURLString(m_urlString);
    m_urlString = QString("http://wiki.anorrl.com/index.php/") + helpTopic;
 
    // url with anchor tag and query - http://wiki.anorrl.com/index.php/Script_Analysis?studiomode=true#W003

	if (FFlag::StudioNewWiki)
	{
		int index = m_urlString.indexOf('#');
		index > 0 ? m_urlString.insert(index, "?studiomode=true") : (m_urlString += "?studiomode=true");
    }

	// if URL has only anchor tag change then do not reset timer as 'loadFinished' signal does not get emitted to remove the watermark
	// instead directly load URL
	if (prevURLString.mid(0, prevURLString.indexOf('#')) != m_urlString.mid(0, m_urlString.indexOf('#')))
	{
		m_pWebView->resetLoadingTimer();
		m_urlDirty = true;
	}
	else
	{
		m_urlDirty = true;
		updateURL();
	}

	update();
}

void ANORRLContextualHelp::updateURL()
{
    if (m_urlDirty)
    {
        m_pWebView->load(m_urlString);
        m_urlDirty = false;
    }
}

ANORRLHelpWebView::ANORRLHelpWebView(ANORRLContextualHelp* widget)
: ANORRLBrowser(widget)
, m_contextualHelpWidget(widget)
{
    connect(this, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
}

void ANORRLHelpWebView::loadProgress(int)
{
    // Run some special code to hide the sidebars and turn off the background.
    // This is temporary until the wiki has Studio friendly view.
    //((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("content = document.getElementById('column-content'); content.style.float = 'none'; content.style.margin = '-3em 0 0 -12.2em'; content.style.width = 'auto'; columnOne = document.getElementById('column-one'); columnOne.style.display = 'none'; document.body.style.background = 'white';");
    //update();
}


void ANORRLHelpWebView::paintEvent(QPaintEvent* event)
{
    m_contextualHelpWidget->updateURL();
    
    ANORRLBrowser::paintEvent(event);
}


