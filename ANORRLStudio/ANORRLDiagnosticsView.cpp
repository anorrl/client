/**
 * ANORRLDiagnosticsView.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLDiagnosticsView.h"

// Qt Headers
#include <QTimer>
#include <QHeaderView>

// ANORRL Headers
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Stats.h"
#include "util/Profiling.h"

static const int UpdateInterval = 1000 / 5;

ANORRLDiagnosticsViewItem::ANORRLDiagnosticsViewItem(boost::shared_ptr<ARL::Stats::Item> pItem)
: m_pItem(pItem)
{
	//ARL::DataModel::LegacyLock lock(getTreeWidget()->dataModel(), ARL::DataModelJob::Read);
	setText(0, m_pItem->getName().c_str());
	setText(1, m_pItem->getStringValue().c_str());

	m_pItem->visitChildren(boost::bind(&ANORRLDiagnosticsViewItem::onChildAdded, this, _1));
	m_childAddedConnection = m_pItem->getOrCreateChildAddedSignal()->connect(boost::bind(&ANORRLDiagnosticsViewItem::onChildAdded, this, _1));
	m_childRemovedConnection = m_pItem->getOrCreateChildRemovedSignal()->connect(boost::bind(&ANORRLDiagnosticsViewItem::onChildRemoved, this, _1));
}

ANORRLDiagnosticsView *ANORRLDiagnosticsViewItem::getTreeWidget()
{	return dynamic_cast<ANORRLDiagnosticsView *>(treeWidget()); }

ANORRLDiagnosticsViewItem* ANORRLDiagnosticsViewItem::getItemParent()
{	return dynamic_cast<ANORRLDiagnosticsViewItem*>(parent()); }

void ANORRLDiagnosticsViewItem::onChildAdded(boost::shared_ptr<ARL::Instance> child)
{
	if(!child)
		return;

	boost::shared_ptr<ARL::Stats::Item> item = ARL::shared_dynamic_cast<ARL::Stats::Item>(child);
	if (!item)
		return;

	ANORRLDiagnosticsViewItem* pViewItem = new ANORRLDiagnosticsViewItem(item);
	if(pViewItem)
	{
		addChild(pViewItem);
	}
}

void ANORRLDiagnosticsViewItem::onChildRemoved(boost::shared_ptr<ARL::Instance> inst)
{
	if(!inst)
		return;

	boost::shared_ptr<ARL::Stats::Item> item = ARL::shared_dynamic_cast<ARL::Stats::Item>(inst);
	if (!item)
		return;

	ANORRLDiagnosticsViewItem* pItemToDelete = NULL;
	for(int ii=0; ii<childCount(); ii++)
	{
		ANORRLDiagnosticsViewItem* pViewItem = dynamic_cast<ANORRLDiagnosticsViewItem*>(child(ii));
		if(pViewItem->getItem() == item)
		{
			pItemToDelete = dynamic_cast<ANORRLDiagnosticsViewItem*>(takeChild(ii));
			break;
		}
	}

	if(pItemToDelete)
	{
		delete pItemToDelete;
		pItemToDelete = NULL;
	}

	if (!isExpanded() && childCount() > 0)					
		setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
}

void ANORRLDiagnosticsViewItem::updateValues()
{
	setText(1, m_pItem->getStringValue().c_str());

	for(int ii=0; ii<childCount(); ii++)
	{
		ANORRLDiagnosticsViewItem* pViewItem = dynamic_cast<ANORRLDiagnosticsViewItem*>(child(ii));
		if(pViewItem)
			pViewItem->updateValues();
	}
}

ANORRLDiagnosticsView::ANORRLDiagnosticsView(bool createdFromIDEDoc)
: m_pTimer(new QTimer(this))
, m_bPreviousProfiling(false)
, m_bIsCreatedFromIDEDoc(createdFromIDEDoc)
{
	setUniformRowHeights(true);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setAlternatingRowColors(true);

	setColumnCount(2);
	QStringList headerLabels;
	headerLabels << "Name" << "Value";
	setHeaderLabels(headerLabels);

	header()->setDefaultSectionSize(125);

    m_pTimer->setInterval(UpdateInterval);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(updateValues()));
}

ANORRLDiagnosticsView::~ANORRLDiagnosticsView()
{
	// turn off profiling
	if (!m_bIsCreatedFromIDEDoc)
		ARL::Profiling::setEnabled(false);

//	setDataModel(shared_ptr<ARL::DataModel>());
}

void ANORRLDiagnosticsView::setDataModel(boost::shared_ptr<ARL::DataModel> pDataModel)
{
	if(m_pDataModel == pDataModel)
		return;

	m_pTimer->stop();

	if (m_pDataModel)
	{
		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
		m_childAddedConnection.disconnect();
		m_childRemovedConnection.disconnect();
		clear();
	}

	m_pStats.reset();
	m_pDataModel = pDataModel;

	if (m_pDataModel)
	{
		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
		m_pStats = shared_from(ARL::ServiceProvider::create<ARL::Stats::StatsService>(m_pDataModel.get()));
		m_pTimer->start();
		
		m_pStats->visitChildren(boost::bind(&ANORRLDiagnosticsView::onChildAdded, this, _1));
		m_childAddedConnection = m_pStats->getOrCreateChildAddedSignal()->connect(boost::bind(&ANORRLDiagnosticsView::onChildAdded, this, _1));
		m_childRemovedConnection = m_pStats->getOrCreateChildRemovedSignal()->connect(boost::bind(&ANORRLDiagnosticsView::onChildRemoved, this, _1));
	} 
}

boost::shared_ptr<ARL::DataModel> ANORRLDiagnosticsView::dataModel()
{	return m_pDataModel; }

void ANORRLDiagnosticsView::updateValues()
{
    // if the window is hidden, stop the update timer
    if ( !isVisible() )
    {
        m_pTimer->stop();
        return;
    }

	for(int ii=0; ii<topLevelItemCount(); ii++)
	{
		ANORRLDiagnosticsViewItem* pViewItem = dynamic_cast<ANORRLDiagnosticsViewItem*>(topLevelItem(ii));
		if(pViewItem)
			pViewItem->updateValues();
	}
}

void ANORRLDiagnosticsView::setVisible(bool visible)
{
	if(visible)
	{
		m_bPreviousProfiling = ARL::Profiling::isEnabled();
		if(!m_bPreviousProfiling)
			ARL::Profiling::setEnabled(true);

		m_pTimer->start();
	}
	else
	{
		ARL::Profiling::setEnabled(m_bPreviousProfiling);
		m_pTimer->stop();
	}
	QTreeWidget::setVisible(visible);
}

void ANORRLDiagnosticsView::onChildAdded(boost::shared_ptr<ARL::Instance> child)
{
	if(!child)
		return;

	boost::shared_ptr<ARL::Stats::Item> item = ARL::shared_dynamic_cast<ARL::Stats::Item>(child);
	if (!item)
		return;
	
	ANORRLDiagnosticsViewItem* pViewItem = new ANORRLDiagnosticsViewItem(item);
	if(pViewItem)
	{
		addTopLevelItem(pViewItem);
	}
}

void ANORRLDiagnosticsView::onChildRemoved(boost::shared_ptr<ARL::Instance> child)
{
	if(!child)
		return;

	boost::shared_ptr<ARL::Stats::Item> item = ARL::shared_dynamic_cast<ARL::Stats::Item>(child);
	if (!item)
		return;

	ANORRLDiagnosticsViewItem* pItemToDelete = NULL;
	for(int ii=0; ii<topLevelItemCount(); ii++)
	{
		ANORRLDiagnosticsViewItem* pViewItem = dynamic_cast<ANORRLDiagnosticsViewItem*>(topLevelItem(ii));
		if(pViewItem->getItem() == item)
		{
			pItemToDelete = dynamic_cast<ANORRLDiagnosticsViewItem*>(takeTopLevelItem(ii));
			break;
		}
	}

	if(pItemToDelete)
	{
		delete pItemToDelete;
		pItemToDelete = NULL;
	}
}



