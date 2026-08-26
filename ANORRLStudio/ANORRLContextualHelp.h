/**
 * ANORRLContextualHelp.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

#include "ANORRLBrowser.h"
#include "ANORRLWebPage.h"

#include <QWidget>

class ANORRLContextualHelpService : public QObject
{
    Q_OBJECT
public:
    ANORRLContextualHelpService();
    
    static ANORRLContextualHelpService& singleton();

public Q_SLOTS:
    void onHelpTopicChanged(const QString& helpTopic);

Q_SIGNALS:
    void helpTopicChanged(const QString&);

private:
    QString m_helpTopic;
};

class ANORRLContextualHelp;

class ANORRLHelpWebView : public ANORRLBrowser
{
    Q_OBJECT
public:
    ANORRLHelpWebView(ANORRLContextualHelp* widget);

protected:
    void paintEvent(QPaintEvent* event);

private Q_SLOTS:
    void loadProgress(int);

private:
    ANORRLContextualHelp*    m_contextualHelpWidget;
};

class ANORRLContextualHelp : public QWidget
{
	Q_OBJECT
	
public:
    ANORRLContextualHelp();

    void updateURL();

public Q_SLOTS:
    void onAuthenticationChanged(bool isAuthenticated);
    void onHelpTopicChanged(const QString& helpTopic);
    
private Q_SLOTS:
	void linkClicked(const QUrl& url);
    
private:
    void setupWebView();
   
    ANORRLHelpWebView*      m_pWebView;
    ANORRLWebPage*          m_pWebPage;
    QString                 m_urlString;
    bool                    m_urlDirty;
};

