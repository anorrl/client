#pragma once

#include "V8DataModel/DataModelJob.h"

namespace ARL {

class SleepingJob : public DataModelJob
{
private:
	rbx::atomic<int> isAwake;
	const double desiredFps;
	ARL::Time lastWakeTime;

public:
	SleepingJob(const char* name, TaskType taskType, bool isPerPlayer,
		shared_ptr<ARL::DataModelArbiter> arbiter, ARL::Time::Interval stepBudget,
		double desiredFps);

	void wake();
	void sleep();

	virtual ARL::Time::Interval sleepTime(const Stats&);

	virtual ARL::TaskScheduler::Job::Error error(const Stats& stats);
};

}
