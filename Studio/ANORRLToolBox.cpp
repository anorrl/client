/**
 * ANORRLToolBox.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLToolBox.h"

// Qt Headers
#include <QGridLayout>
#include <QWebChannel>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

// ANORRL Headers
#include "v8datamodel/DataModel.h"
#include "v8datamodel/InsertService.h"
#include "v8datamodel/ContentProvider.h"
#include "util/standardout.h"
#include "network/Players.h"

// ANORRL Studio Headers
#include "AuthenticationHelper.h"
#include "ARLWorkspace.h"
#include "ANORRLCookieJar.h"
#include "ANORRLNetworkAccessManager.h"
#include "ANORRLSettings.h"

FASTFLAG(WebkitLocalStorageEnabled);
FASTFLAG(WebkitDeveloperToolsEnabled);
FASTFLAG(StudioEnableWebKitPlugins);

ANORRLToolBox::ANORRLToolBox()
: m_pWorkspace()
, m_pWebView(NULL)
, reloadView(false)
{
	QGridLayout *layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	
    setMaximumWidth(250);

	setupWebView(this);
	layout->addWidget(m_pWebView, 0, 0);
	setLayout(dynamic_cast<QLayout*>(layout));

    setMaximumWidth(QWIDGETSIZE_MAX);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	
	setMinimumWidth(285);
}

void ANORRLToolBox::setupWebView(QWidget *wrapperWidget)
{
	m_pWebView = new ANORRLBrowser(wrapperWidget);
	m_pWebPage = new ANORRLWebPage(wrapperWidget);
	
	m_pWebView->setPage(m_pWebPage);
	connect(m_pWebView->page(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(initJavascript())); 

	m_urlString = QString("%1/ide/toolbox").arg(ANORRLSettings::getBaseURL());

	connect(&AuthenticationHelper::Instance(), SIGNAL(authenticationChanged(bool)), this, SLOT(onAuthenticationChanged(bool)));
}


void ANORRLToolBox::setDataModel(boost::shared_ptr<ARL::DataModel> pDataModel)
{
	if(m_pDataModel == pDataModel)
		return;

    bool firstTime = false;
    m_pDataModel = pDataModel;

    if (!m_pWorkspace)
    {
        m_pWorkspace.reset(new ARLWorkspace(this, m_pDataModel ? m_pDataModel.get() : NULL));
        firstTime = true;
    }
    else
    {
        m_pWorkspace->setDataModel(pDataModel.get());
    }

    if (!m_pDataModel)
	{
		setEnabled(false);
		return;
	}

	setEnabled(true);

    if (firstTime)
	{
        m_pWebView->load(m_urlString);
	}
	else if (reloadView)
	{
		m_pWebPage->triggerAction(QWebEnginePage::Reload);
	}
	reloadView = false;
    
	update();
}

void ANORRLToolBox::initJavascript()
{
	if(m_pWorkspace && m_pWebView->page())
	{
		QWebChannel* pWebChannel = new QWebChannel(m_pWebView->page());
		pWebChannel->registerObject(QStringLiteral("external"), m_pWorkspace.get());
		m_pWebView->page()->setWebChannel(pWebChannel);

		m_pWebView->page()->runJavaScript(
			QStringLiteral("new QWebChannel(qt.webChannelTransport, function(channel) { window.external = channel.objects.external; });"));
	}
}

QString ANORRLToolBox::getTitleFromUrl(const QString &urlString)
{	
	/*if(m_pWebView && m_pWebView->page())
	{
		int pos = urlString.indexOf("id=");
		if (pos > 0 && pos+3 < urlString.size())
		{
			QWebElementCollection toolboxItemElements = m_pWebView->page()->mainFrame()->findAllElements(QString("span#span_setitem_%1 a").arg(urlString.mid(pos+3)));
			Q_FOREACH(QWebElement toolboxItemElement, toolboxItemElements)
			{
				QStringList attributesList = toolboxItemElement.attributeNames();
				Q_FOREACH(QString attributeName, attributesList)
				{
					if (attributeName == "title")
						return toolboxItemElement.attribute(attributeName);
				}
			}
		}
	}*/
	
	return QString("");
}

void ANORRLToolBox::onAuthenticationChanged(bool)
{
	if (m_pDataModel)
		m_pWebPage->triggerAction(QWebEnginePage::Reload);
	else
		reloadView = true;
}

void ANORRLToolBox::loadUrl(const QString url)
{
	setEnabled(true);
	m_urlString = url;
	m_pWebView->load(url);
}