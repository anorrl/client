/**
 * RobloxDiagnosticsView.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

//this will make sure that we can use qt with boost (else moc compiler errors)
#ifndef QT_NO_KEYWORDS
    #define QT_NO_KEYWORDS
#endif

// Qt Headers
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMutex>

// Roblox Headers
#include "rbx/signal.h"

namespace ARL {
	class Instance;
	class DataModel;
	namespace Stats
	{
		class Item;
		class StatsService;
	}
}

class RobloxDiagnosticsView : public QTreeWidget
{
	Q_OBJECT
public:
	RobloxDiagnosticsView(bool createdFromIDEDoc = false); 
	virtual ~RobloxDiagnosticsView();

	void setDataModel(boost::shared_ptr<ARL::DataModel> pDataModel);
	boost::shared_ptr<ARL::DataModel> dataModel();	
	
public Q_SLOTS:
	void setVisible(bool visible);

private Q_SLOTS:
	void updateValues();
		
private:
	void onChildAdded(boost::shared_ptr<ARL::Instance> child);
	void onChildRemoved(boost::shared_ptr<ARL::Instance> child);

	boost::shared_ptr<ARL::DataModel>				m_pDataModel;
	boost::shared_ptr<ARL::Stats::StatsService>		m_pStats;
	rbx::signals::scoped_connection					m_childAddedConnection;
	rbx::signals::scoped_connection					m_childRemovedConnection;
	QTimer										   *m_pTimer;

	bool											m_bPreviousProfiling;
	bool                                            m_bIsCreatedFromIDEDoc;
};

class RobloxDiagnosticsViewItem: public QTreeWidgetItem
{
public:
	RobloxDiagnosticsViewItem(boost::shared_ptr<ARL::Stats::Item> pItem);

	RobloxDiagnosticsView     *getTreeWidget();
	RobloxDiagnosticsViewItem *getItemParent();

	boost::shared_ptr<ARL::Stats::Item> getItem() { return m_pItem; }

	void updateValues();

private:
	void onChildAdded(boost::shared_ptr<ARL::Instance> child);
	void onChildRemoved(boost::shared_ptr<ARL::Instance> child);

	boost::shared_ptr<ARL::Stats::Item>		m_pItem;
	rbx::signals::scoped_connection			m_childAddedConnection;
	rbx::signals::scoped_connection			m_childRemovedConnection;	
};

