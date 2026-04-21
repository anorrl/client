
#include "rbx/Tasks/Coordinator.h"

using namespace ARL;
using namespace Tasks;

bool Barrier::isInhibited(TaskScheduler::Job* job)
{
	ARL::mutex::scoped_lock lock(mutex);
	return jobs[job] > counter;
}

void Barrier::onPostStep(TaskScheduler::Job* job)
{
	ARL::mutex::scoped_lock lock(mutex);

	unsigned int& count(jobs[job]);

	// Note: we could almost assert that count==counter, but
	//       there may be edge cases where a step slips through
	//       the cracks
	if (count == counter)
	{
		count++;
		if (--remainingTasks == 0)
			releaseBarrier();
	}
}

void Barrier::releaseBarrier()
{
	remainingTasks = int(jobs.size());
	counter++;
}

void Barrier::onAdded(TaskScheduler::Job* job)
{
	ARL::mutex::scoped_lock lock(mutex);

	ARLASSERT(jobs.find(job)==jobs.end());

	jobs[job] = counter;
	remainingTasks++;

	ARLASSERT(remainingTasks <= jobs.size());
}

void Barrier::onRemoved(TaskScheduler::Job* job)
{
	ARL::mutex::scoped_lock lock(mutex);

	ARLASSERT(jobs.find(job)!=jobs.end());

	if (jobs[job] <= counter)
		remainingTasks--;

	jobs.erase(job);

	ARLASSERT(remainingTasks <= jobs.size());
}




bool SequenceBase::isInhibited(TaskScheduler::Job* job)
{
	ARL::mutex::scoped_lock lock(mutex);

	if (nextJobIndex < jobs.size())
		return jobs[nextJobIndex] != job;
	else
		return true;
}

void SequenceBase::advance()
{
	ARL::mutex::scoped_lock lock(mutex);

	if (++nextJobIndex == jobs.size())
		nextJobIndex = 0;
}

void SequenceBase::onAdded(TaskScheduler::Job* job)
{
	ARL::mutex::scoped_lock lock(mutex);

	jobs.push_back(job);
}

void SequenceBase::onRemoved(TaskScheduler::Job* job)
{
	ARL::mutex::scoped_lock lock(mutex);

	for (size_t i=0; i<jobs.size(); ++i)
		if (jobs[i] == job)
		{
			jobs.erase(jobs.begin() + i);
			if (nextJobIndex > i)
				--nextJobIndex;
			break;
		}
}


bool Exclusive::isInhibited(TaskScheduler::Job* job)
{
	ARLASSERT(job != runningJob);
	return runningJob != NULL;
}

void Exclusive::onPreStep(TaskScheduler::Job* job)
{
	ARLASSERT(runningJob == NULL);
	runningJob = job;
}

void Exclusive::onPostStep(TaskScheduler::Job* job)
{
	ARLASSERT(runningJob == job);
	runningJob = NULL;
}





