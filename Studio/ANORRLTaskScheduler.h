/**
 * ANORRLTaskScheduler.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// ANORRL Headers
#include "arl/TaskScheduler.h"

// ANORRL Studio Headers
#include "ANORRLReportView.h"

class TSJobItem;

class ArbiterItem: public ANORRLCategoryItem
{
public:
	ArbiterItem() {}
	TSJobItem* getOrCreateTSJobItem(boost::shared_ptr<const ARL::TaskScheduler::Job> pJob);
};

class TSJobItem: public QTreeWidgetItem
{
public:
	TSJobItem(ArbiterItem* pParentItem, boost::shared_ptr<const ARL::TaskScheduler::Job> pJob);

	boost::shared_ptr<const ARL::TaskScheduler::Job> getJob() { return m_pJob; }
	void updateValues();

private:
	boost::shared_ptr<const ARL::TaskScheduler::Job>	m_pJob;
};

class ANORRLTaskScheduler : public ANORRLReportView
{
	Q_OBJECT
public:
	ANORRLTaskScheduler(); 
	virtual ~ANORRLTaskScheduler();

public Q_SLOTS:
	void setVisible(bool visible);

private Q_SLOTS:
	void updateValues();

private:
	typedef std::vector<boost::shared_ptr<const ARL::TaskScheduler::Job> > Jobs;
	typedef std::map<boost::shared_ptr<ARL::TaskScheduler::Arbiter>, ArbiterItem*> ArbiterItemsMap;
	typedef std::set<TSJobItem*> TSJobItemCollection;

	ArbiterItem* getOrCreateArbiterItem(boost::shared_ptr<ARL::TaskScheduler::Arbiter> pArbiter);
	void syncTSJobItems(const TSJobItemCollection &currentTaskSchedulerItems);

	ArbiterItemsMap				 m_arbiterMap;
	TSJobItemCollection          m_TSJobItems;

	QTimer						*m_pTimer;
};


