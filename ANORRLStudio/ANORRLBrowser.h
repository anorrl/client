/**
 * ANORRLBrowser.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QWebView>

class ANORRLBrowser : public QWebView 
{
	Q_OBJECT

public: 

	ANORRLBrowser(QWidget* parent=0);
    virtual ~ANORRLBrowser();
	bool neverReloadOnAuth;

public Q_SLOTS:

	bool close();
    void resetLoadingTimer();
    
protected:
    
    virtual void paintEvent(QPaintEvent*);
 	virtual void dropEvent(QDropEvent* evt);

private Q_SLOTS:

	void onAuthenticationChanged(bool);
    void loadStarted();
    void loadFinished(bool);
    
private:
    
    void drawLoadingWatermark();
    
	ANORRLBrowser*  m_pPopup;
	QDialog*        m_pPopupDlg;
    QTimer*         m_loadingTimer;
    float           m_refreshIncr;
};