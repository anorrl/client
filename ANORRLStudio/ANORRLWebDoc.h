/**
 * ANORRLWebDoc.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QObject>

#include <boost/shared_ptr.hpp>

// ANORRL Studio Headers
#include "ANORRLBasicDoc.h"

class QComboBox;
class QNetworkReply;
class QUrl;
class QToolBar;
class QSslError;

class ANORRLBrowser;
class ARLWorkspace;

class ANORRLWebDoc: public QObject, public ANORRLBasicDoc
{
	Q_OBJECT
public:

	ANORRLWebDoc(const QString& displayName, const QString& keyName);
	~ANORRLWebDoc();
	
	bool open(ANORRLMainWindow *pMainWindow, const QString &fileName);
	
	IANORRLDoc::ARLCloseRequest requestClose() { return IANORRLDoc::NO_SAVE_NEEDED; }

	IANORRLDoc::ARLDocType docType() { return IANORRLDoc::BROWSER; }

	QString fileName() const        { return ""; }
	QString displayName() const     { return m_displayName; }
	QString keyName() const         { return m_keyName; }
			
	bool save(){ return false; }
	bool saveAs(const QString &){ return false; }

	QString saveFileFilters() { return ""; }
	
	QWidget* getViewer();
	
	bool isModified(){ return false; }
	
	void activate();
	void deActivate() { m_bActive = false; }
		
	bool actionState(const QString &actionID, bool &enableState, bool &checkedState);

	bool handlePluginAction(void *, void *) { return false; }

	void handleScriptCommand(const QString &) { return; }	

	bool supportsZeroPlaneGrid()        { return false; }

	boost::shared_ptr<ARLWorkspace> getWorkspace() { return m_pWorkspace; }

public Q_SLOTS:
	void refreshPage();
	
private Q_SLOTS:
	void updateAddressBar(const QUrl&);
	void sslErrorHandler(QNetworkReply* qnr, const QList<QSslError> & errlist);
	void initJavascript();
	void handleHomeClicked();
	void navigateUrl(const QString& url);
	void onAuthenticationChanged(bool);
	void updateTitle(QString title);

private:
	virtual bool doClose();
	void setupWebView(QWidget*);
	QToolBar* setupAddressToolBar(QWidget*);
	

	ANORRLBrowser                       *m_pWebView;
	QWidget                             *m_pWrapperWidget;
	boost::shared_ptr<ARLWorkspace>      m_pWorkspace;
	QComboBox                           *m_pAddrInputComboBox;
	QString                              m_displayName; //to be used if file name is empty
	QString                              m_currentUrl;
	QString                              m_homeUrl;
	QString                              m_keyName;
};

