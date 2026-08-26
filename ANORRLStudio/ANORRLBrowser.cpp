/**
 * ANORRLBrowser.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLBrowser.h"
#include "FastLog.h"

// Qt Headers
#include <QContextMenuEvent>
#include <QDialog>
#include <QFileInfo>
#include <QWebEngineView>
#include <QMimeData>

// ANORRL Studio Headers
#include "ANORRLMainWindow.h"
#include "UpdateUIManager.h"

LOGVARIABLE(BrowserActivity, 0)
FASTFLAGVARIABLE(WebkitLocalStorageEnabled, false)
FASTFLAGVARIABLE(WebkitDeveloperToolsEnabled, false)

ANORRLBrowser::ANORRLBrowser(QWidget* parent) 
    : QWebEngineView(parent)
    , m_pPopup(NULL)
    , m_pPopupDlg(NULL)
    , m_loadingTimer(NULL)
    , m_refreshIncr(0.0f)
{
    connect(this, SIGNAL(loadStarted()), this, SLOT(loadStarted()));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
    //setAcceptDrops(true);
}

ANORRLBrowser::~ANORRLBrowser()
{
}

void ANORRLBrowser::dropEvent(QDropEvent *evt)
{
	const QMimeData *pMimeData = evt->mimeData();

	if (pMimeData && pMimeData->hasUrls())
	{
		QList<QUrl> urlList = pMimeData->urls();
		for (int i = 0; i < urlList.size() && i < 6; ++i) 
		{
			QString fileName = urlList.at(i).toLocalFile();
			if (!fileName.isEmpty())
			{
				if (fileName.endsWith(".arl", Qt::CaseInsensitive) || fileName.endsWith(".arlx", Qt::CaseInsensitive))
				{
					UpdateUIManager::Instance().getMainWindow().handleFileOpen(fileName, IANORRLDoc::IDE);
				}
			}
		}
	}

	evt->acceptProposedAction();
}

bool ANORRLBrowser::close()
{
	if(m_pPopupDlg) 
		m_pPopupDlg->close();
	return true; 
}

void ANORRLBrowser::loadStarted()
{
    FASTLOGS(FLog::BrowserActivity, "URL: %s", url().toString().toStdString().c_str());
    resetLoadingTimer();
}

void ANORRLBrowser::loadFinished(bool)
{
    if (m_loadingTimer)
    {
        m_loadingTimer->stop();
        delete m_loadingTimer;
        m_loadingTimer = NULL;
    }
    
    update();
}

void ANORRLBrowser::resetLoadingTimer()
{
    if (m_loadingTimer)
    {
        m_loadingTimer->stop();
        delete m_loadingTimer;
        m_loadingTimer = NULL;
    }
    
    m_loadingTimer = new QTimer(this);
    connect(m_loadingTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_loadingTimer->start(1000/30);
    m_refreshIncr = 0;
}