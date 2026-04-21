/**
 * RobloxScriptReview.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

// Standard C/C++ Headers
#include <map>

// 3rd Party Headers
#include "boost/shared_ptr.hpp"

// Roblox Studio Headers
#include "RobloxReportView.h"

namespace ARL 
{
	class DataModel;
	namespace Reflection
	{
		class Variant;
	}
}

class QTreeWidgetItem;

typedef std::map<std::string, QTreeWidgetItem*>	RecordMap;

class RobloxScriptReview : public RobloxReportView
{
	Q_OBJECT

public:

	RobloxScriptReview(); 
	virtual ~RobloxScriptReview();

	class ScriptPerfUpdateJob;

	void setDataModel(boost::shared_ptr<ARL::DataModel> pDataModel);
	boost::shared_ptr<ARL::DataModel> dataModel();	

	virtual void requestUpdate();

public Q_SLOTS:

	void setVisible(bool visible);

protected:

	void AddValue(const ARL::Reflection::Variant& value);
	void createUpdateItemsJob(boost::shared_ptr<ARL::DataModel> dataModel);
	void deleteUpdateItemsJob();

private Q_SLOTS:

	void update();
	void onTimer();
	void onSectionClicked(int index);
	
private:

	virtual bool event(QEvent * evt);

	RecordMap                               m_records;

	boost::shared_ptr<ARL::DataModel>	    m_pDataModel;
	boost::shared_ptr<ScriptPerfUpdateJob>  m_pUpdateItemsJob;
	
	QTimer*                                 m_pTimer;
};


