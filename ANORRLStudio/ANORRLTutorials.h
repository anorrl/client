/**
 * ANORRLTutorials.h
 * Copyright (c) 2014 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

#include <QObject>
#include <QWidget>
#include <QPushButton>

class QUrl;
class QToolBar;

class ANORRLBrowser;

class ANORRLTutorials: public QWidget
{
	Q_OBJECT

public:
	ANORRLTutorials();

private Q_SLOTS:
	void linkClicked(const QUrl& url);
	void urlChanged(const QUrl& url);
	void disableLinks(const QUrl& url);
	void updateButtons();
	void handleHomeClicked();

	void resizeWidget();

private:
	void setupWebView(QWidget*);

	ANORRLBrowser                       *m_pWebView;
	ANORRLBrowser						*m_pWebContentsView;
	QWidget                             *m_pWrapperWidget;
	QPushButton							*m_pBackButton;
	QPushButton							*m_pForwardButton;
	QPushButton							*m_pStopButton;
	QPushButton							*m_pReloadButton;
	QPushButton							*m_pHomeButton;

	bool								m_firstPaint;
	bool								m_firstClick;
};

