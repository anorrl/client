#include "stdafx.h"

#include "V8DataModel/SleepingJob.h"

namespace ARL {

SleepingJob::SleepingJob(const char* name, TaskType taskType, bool isPerPlayer,
		shared_ptr<ARL::DataModelArbiter> arbiter, ARL::Time::Interval stepBudget,
		double desiredFps)
	: ARL::DataModelJob(name, taskType, isPerPlayer, arbiter, stepBudget)
	, desiredFps(desiredFps) {
	isAwake = false;
}

void SleepingJob::wake() { 
	if (!isAwake.compare_and_swap(1,0))
	{
		lastWakeTime = ARL::Time::now<ARL::Time::Fast>();
		ARL::TaskScheduler::singleton().reschedule(shared_from_this());
	}
}

void SleepingJob::sleep() {
	isAwake = false;
}

Time::Interval SleepingJob::sleepTime(const Stats&) {
	return isAwake ? ARL::Time::Interval(0) : ARL::Time::Interval::max();
}

TaskScheduler::Job::Error SleepingJob::error(const Stats& stats) {
	if (!isAwake)
		return Job::Error();
	
	Stats fakedStats = stats;
	ARL::Time::Interval timeSinceAwoke = ARL::Time::now<ARL::Time::Fast>() - lastWakeTime;
	if (timeSinceAwoke < fakedStats.timespanSinceLastStep) {
		fakedStats.timespanSinceLastStep = timeSinceAwoke;
	}

	return computeStandardError(fakedStats, desiredFps);
}

}