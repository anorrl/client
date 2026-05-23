/**
 * ANORRLToolBox.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

#include <boost/shared_ptr.hpp>

// ANORRL Headers
#include "reflection/Type.h"

// ANORRL Studio Headers
#include "ANORRLBrowser.h"
#include "ANORRLWebPage.h"

namespace ARL
{
	class DataModel;
}

class ARLWorkspace;


class ANORRLToolBox : public QWidget
{
	Q_OBJECT
	
public:
	ANORRLToolBox();
	void setDataModel(boost::shared_ptr<ARL::DataModel> pDataModel);
	QString getTitleFromUrl(const QString &urlString);
	
public Q_SLOTS:
	void onAuthenticationChanged(bool isAuthenticated);
	void loadUrl(const QString url);

private:
	ANORRLBrowser *m_pWebView;
	ANORRLWebPage *m_pWebPage;
	boost::shared_ptr<ARL::DataModel>	m_pDataModel;
	
	QString m_urlString;
	
	boost::shared_ptr<ARLWorkspace> m_pWorkspace;
	
	void setupWebView(QWidget *wrapperWidget);
	bool reloadView;
	
private Q_SLOTS:
	void initJavascript();
	
};
