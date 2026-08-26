/**
 * WebDialog.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QDialog>
#include "ANORRLSavingStateDialog.h"

#include <boost/shared_ptr.hpp>

class ANORRLBrowser;
class ANORRLWebPage;
class ARLWorkspace;

namespace ARL
{
	class DataModel;
}
class WebDialog : public ANORRLSavingStateDialog<QDialog>
{
	Q_OBJECT

public:

    WebDialog(QWidget *parent, const QString& initialUrl, ARL::DataModel*, int widthInPixels = 800, int heightInPixels = 600);

	void load(const QString& url);

	ANORRLWebPage* getWebPage() const { return m_pWebPage; }

protected:
	QString m_initialUrl;
	ANORRLBrowser		*m_pWebView;
	ANORRLWebPage		*m_pWebPage;
	ARL::DataModel		*m_pDataModel;

	boost::shared_ptr<ARLWorkspace> m_pWorkspace;

    virtual QSize sizeHint() const;
	virtual void resizeEvent(QResizeEvent* evt);
	
private Q_SLOTS:
	void initJavascript();

private:
	int m_iWidthInPixels;
	int m_iHeightInPixels;
};

class UploadDialog : public WebDialog
{
public:

	enum UploadRequestResult
	{
		OK, LOCAL_SAVE, CANCEL
	};

	UploadDialog(QWidget* parent, ARL::DataModel*);

private:
    virtual void reject();
	virtual void closeEvent(QCloseEvent* evt);
};

class ImportAssetDialog : public WebDialog
{
public:
	ImportAssetDialog(QWidget* parent, const QString& initialUrl, ARL::DataModel* dataModel)
	: WebDialog(parent, initialUrl, dataModel)
	, m_AssetId(-1)
	{
	}

	int getAssetId()  { return m_AssetId; }
	void setAssetId(int assetId) { m_AssetId = assetId; }

private:
	int m_AssetId;
};