/**
 * ANORRLBrowser.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QWebEngineView>

class ANORRLBrowser : public QWebEngineView
{
	Q_OBJECT

public: 

	ANORRLBrowser(QWidget* parent=0);
    virtual ~ANORRLBrowser();

public Q_SLOTS:

	bool close();
    void resetLoadingTimer();
    
protected:
    
    virtual void dropEvent(QDropEvent* evt);

private Q_SLOTS:

    void loadStarted();
    void loadFinished(bool);
    
private:
    
    void drawLoadingWatermark();
    
	ANORRLBrowser*  m_pPopup;
	QDialog*        m_pPopupDlg;
    QTimer*         m_loadingTimer;
    float           m_refreshIncr;
};
