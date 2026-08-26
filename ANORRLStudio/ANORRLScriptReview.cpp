/**
 * ANORRLScriptReview.cpp
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "ANORRLScriptReview.h"

// Qt Headers
#include <QApplication>
#include <QTimer>
#include <QHeaderView>

// ANORRL Headers
#include "script/ScriptContext.h"
#include "v8datamodel/DataModel.h"
#include "arl/TaskScheduler.h"
#include "arl/TaskScheduler.Job.h"
#include "arl/CEvent.h"

// ANORRL Studio Headers
#include "ANORRLCustomWidgets.h"

static const int UpdateInterval = 1000 / 5;

class ANORRLScriptReview::ScriptPerfUpdateJob : public ARL::DataModelJob
{
public:
	ANORRLScriptReview* m_pScriptReview;
	ARL::CEvent update;		// Set when the UI thread may update
	ARL::CEvent updated;	// Set when the UI thread has updated
	volatile bool isGridInvalid;
	typedef enum { SentUpdateMessage, Updating, Waiting } State;
	volatile State m_state;
	double waitingPhaseStep;
	double timeSinceLastRender;

	ScriptPerfUpdateJob(ANORRLScriptReview* scriptReview, boost::shared_ptr<ARL::DataModel> dataModel)
		:ARL::DataModelJob("ANORRLScriptReview", ARL::DataModelJob::Read, false, dataModel, ARL::Time::Interval(0)),
		m_pScriptReview(scriptReview),
		update(false),
		updated(false),
		isGridInvalid(true),
		m_state(Waiting)
	{}
	
	void requestUpdate()
	{
		if (!isGridInvalid)
		{
			isGridInvalid = true;
			ARL::TaskScheduler::singleton().reschedule(shared_from_this());
		}
	}

	void setState(ScriptPerfUpdateJob::State state)
	{ m_state = state; }

	virtual ARL::Time::Interval sleepTime(const Stats& stats)
	{
		switch (m_state)
		{
		default:
		case Waiting:
			{
				if (isGridInvalid)
					return ARL::Time::Interval::zero();
				else
					return ARL::Time::Interval::max();
			}

		case Updating:
			return ARL::Time::Interval::zero();

		case SentUpdateMessage:
			return ARL::Time::Interval::max();
		}
	}

	virtual Job::Error error(const Stats& stats)
	{
		switch (m_state)
		{
		default:
		case Waiting:
			{
				ARLASSERT(isGridInvalid);
				waitingPhaseStep = stats.timespanSinceLastStep.seconds();
				Job::Error result;
				result.error = waitingPhaseStep;
				return result;
			}

		case Updating:
			{
				Job::Error result;
			    // The error is the sum of the previous phase plus this phase
				result.error = waitingPhaseStep + stats.timespanSinceLastStep.seconds();	
				result.urgent = true;
				return result;
			}

		case SentUpdateMessage:
			ARLASSERT(false);
			return Job::Error();
		}
	}

	virtual ARL::TaskScheduler::StepResult stepDataModelJob(const Stats& stats)
	{
		timeSinceLastRender = waitingPhaseStep + stats.timespanOfLastStep.seconds();

		switch (m_state)
		{
		case Waiting:
			ARLASSERT(isGridInvalid);
			m_state = SentUpdateMessage;
			QApplication::postEvent(m_pScriptReview, new ANORRLCustomEvent(SCRIPT_REVIEW_UPDATE));
			break;

		case SentUpdateMessage:
			ARLASSERT(false);
			break;

		case Updating:
			update.Set();
			updated.Wait();
			m_state = Waiting;
			break;

		}

		return ARL::TaskScheduler::Stepped;
	}
};

ANORRLScriptReview::ANORRLScriptReview()
: m_pTimer(new QTimer(this))
{
	setColumnCount(4);
	QStringList headerLabels;
	headerLabels << "Name" << "Count" << "Activity" << "Rate";
	setHeaderLabels(headerLabels);
	setIndentation(0);

	header()->setDefaultSectionSize(50);
	header()->resizeSection(0, 100);
	header()->setSectionsClickable(true);

    m_pTimer->setInterval(UpdateInterval);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	connect(header(), SIGNAL(sectionClicked(int)), this, SLOT(onSectionClicked(int)));
}

ANORRLScriptReview::~ANORRLScriptReview()
{}

void ANORRLScriptReview::setDataModel(boost::shared_ptr<ARL::DataModel> pDataModel)
{
	if(m_pDataModel == pDataModel)
		return;

	if (m_pDataModel)
	{
		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
		deleteUpdateItemsJob();
		clear();
	}

	m_records.clear();
	m_pDataModel = pDataModel;

	if (m_pDataModel)
	{
		ARL::DataModel::LegacyLock lock(m_pDataModel, ARL::DataModelJob::Write);
		createUpdateItemsJob(m_pDataModel);
		m_pTimer->start();
	}
	else 
		m_pTimer->stop();

	requestUpdate();
}

void ANORRLScriptReview::AddValue(const ARL::Reflection::Variant& value)
{
	//DE2502 - Do not show objects with name "Unknown" and count -1
	shared_ptr<const ARL::Reflection::Tuple> tuple = value.cast<shared_ptr<const ARL::Reflection::Tuple> >();
	if (tuple->values[1].cast<std::string>() == "[Unknown]" && tuple->values[2].cast<int>() == -1)
		return;
	
	ANORRLCategoryItem* pCategoryItem = findCategoryItem("Data Model: Data Model");
	if(!pCategoryItem)
	{
		pCategoryItem = new ANORRLCategoryItem();
		pCategoryItem->setText(0, "Data Model: Data Model");
		addCategoryItem(pCategoryItem);		
	}
	expandItem(pCategoryItem);

	std::string hash = tuple->values[0].cast<std::string>();
	RecordMap::iterator iter = m_records.find(hash);
	if(iter != m_records.end()){
		QTreeWidgetItem* pRecord = iter->second;
		pRecord->setText(1, ARL::format("%d",	tuple->values[2].cast<int>()).c_str());
		pRecord->setText(2, ARL::format("%.3f%%",tuple->values[3].cast<double>()).c_str());
		pRecord->setText(3, ARL::format("%.1f/s",	tuple->values[4].cast<double>()).c_str());
	}
	else{
		QTreeWidgetItem* pRecord = new QTreeWidgetItem();
		pRecord->setText(0, tuple->values[1].cast<std::string>().c_str());
		pRecord->setText(1, ARL::format("%d", tuple->values[2].cast<int>()).c_str());
		pRecord->setText(2, ARL::format("%.3f%%", tuple->values[3].cast<double>()).c_str());
		pRecord->setText(3, ARL::format("%.1f/s", tuple->values[4].cast<double>()).c_str());
	
		pCategoryItem->addChild(pRecord);
		m_records[hash] = pRecord;
	}
}

void ANORRLScriptReview::update()
{
	if (m_pUpdateItemsJob)
	{
		if (!m_pUpdateItemsJob->isGridInvalid)
			return;

		m_pUpdateItemsJob->setState(ScriptPerfUpdateJob::Updating);
		ARL::TaskScheduler::singleton().reschedule(m_pUpdateItemsJob);
		m_pUpdateItemsJob->update.Wait();		
	}

	setUpdatesEnabled(false);

	boost::shared_ptr<ARL::ScriptContext> pScriptContext = ARL::shared_from(ARL::ServiceProvider::create<ARL::ScriptContext>(m_pDataModel.get()));
	if(pScriptContext)
	{		
		pScriptContext->setCollectScriptStats(true);
		if(shared_ptr<const ARL::Reflection::Tuple> stats = pScriptContext->getScriptStats())
		{
			ARL::Reflection::ValueArray::const_iterator curIter = stats->values.begin();
			while (curIter != stats->values.end())
			{
				AddValue(*curIter);
				++curIter;
			}
		}
	}

	setUpdatesEnabled(true);	

	if (m_pUpdateItemsJob)
	{
		m_pUpdateItemsJob->isGridInvalid = false;
		m_pUpdateItemsJob->updated.Set();	
	}
}

void ANORRLScriptReview::requestUpdate()
{
	if (m_pUpdateItemsJob)
		m_pUpdateItemsJob->requestUpdate();
	else
		QApplication::postEvent(this, new ANORRLCustomEvent(SCRIPT_REVIEW_UPDATE));
}

bool ANORRLScriptReview::event(QEvent * evt)
{
	if (evt->type() != SCRIPT_REVIEW_UPDATE)
		return QTreeWidget::event(evt);

    if ( isEnabled() )
	{
	    update();
	}
	else
	{
		// if widget is not enabled, make sure we again post the event so it can be evaulated whenever the widget is enabled
		QApplication::postEvent(this, new ANORRLCustomEvent(SCRIPT_REVIEW_UPDATE));
	}

	return true;
}

void ANORRLScriptReview::createUpdateItemsJob(boost::shared_ptr<ARL::DataModel> dataModel)
{
	ARLASSERT(!m_pUpdateItemsJob);
	m_pUpdateItemsJob.reset(new ScriptPerfUpdateJob(this, dataModel));
	ARL::TaskScheduler::singleton().add(m_pUpdateItemsJob);
}

void ANORRLScriptReview::deleteUpdateItemsJob()
{
	if (m_pUpdateItemsJob)
	{
		m_pUpdateItemsJob->updated.Set();
		ARL::TaskScheduler::singleton().removeBlocking(m_pUpdateItemsJob);
		m_pUpdateItemsJob.reset();
	}
}

void ANORRLScriptReview::setVisible(bool visible)
{
	if(visible && m_pDataModel)
	{
		m_pTimer->start();
	}
	else
	{
		m_pTimer->stop();
	}
	ANORRLReportView::setVisible(visible);
}

void ANORRLScriptReview::onTimer()
{
    // if the window is hidden, stop the update timer
	if ( !isVisible() )
	{
		m_pTimer->stop();
		return;
	}

	requestUpdate();
}

void ANORRLScriptReview::onSectionClicked(int index)
{
	if(!isSortingEnabled())
		setSortingEnabled(true);
}


