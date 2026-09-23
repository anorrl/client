#pragma once

#include "v8datamodel/GlobalSettings.h"
#include "v8datamodel/DataModelJob.h"
#include "v8world/World.h"

namespace ARL {



	// A generic mechanism for displaying stats (like 3D FPS, network traffic, etc.)
	extern const char *const sDebugSettings;
	class DebugSettings 
		 : public GlobalAdvancedSettingsItem<DebugSettings, sDebugSettings>
	{
	private:
		bool stackTracingEnabled;

		float pixelShaderModel;
		float vertexShaderModel;

		bool reportExtendedMachineConfiguration;

	public:
		bool soundWarnings;
		bool fmodProfiling;
		bool enableProfiling;

		typedef enum { DontReport, Prompt, Report } ErrorReporting;
		ErrorReporting errorReporting;

		static std::string anorrlVersion;
		static std::string anorrlProductName;

		DebugSettings();
		static Reflection::BoundProp<bool> prop_stackTracingEnabled;
		static Reflection::BoundProp<bool> prop_reportExtendedMachineConfiguration;
		static Reflection::BoundProp<bool> prop_ioEnabled;

		bool getStackTracingEnabled() const { return stackTracingEnabled; }

		int getLuaRamLimit() const;
		void setLuaRamLimit(int value);

		int getBlockMeshMapCount() const;

		bool blockingRemove;
		void setBlockingRemove(bool value) { blockingRemove = value; }

		void noOpt() {}

		// "Errors"
		bool getIsProfilingEnabled() const;
		void setIsProfilingEnabled(bool value);
		double getProfilingWindow() const;
		void setProfilingWindow(double value);

		ErrorReporting getErrorReporting() const { return errorReporting; }
		void setErrorReporting(ErrorReporting value);

		bool getReportExtendedMachineConfiguration() const { return reportExtendedMachineConfiguration; };

		Time::SampleMethod getTickCountPreciseOverride() const
		{
			return Time::preciseOverride;
		}
		void setTickCountPreciseOverride(Time::SampleMethod value)
		{
			Time::preciseOverride = value;
		}

		float getVertexShaderModel() const;
		float getPixelShaderModel() const;
		void setVertexShaderModel(float);
		void setPixelShaderModel(float);
		int videoMemory() const; // Mbytes
		int cpuSpeed() const; // MHz
		int cpuCount() const;
		std::string systemProductName() const;
		std::string getANORRLVersion() const { ARLASSERT(!anorrlVersion.empty()); return anorrlVersion; }
		std::string getANORRLProductName() const { ARLASSERT(!anorrlProductName.empty()); return anorrlProductName; }
		std::string osVer() const;
		int osPlatformId() const;
		std::string osPlatform() const;
		std::string deviceName() const;
		bool osIs64Bit() const;
		std::string gfxcard() const;
		std::string cpu() const;
		std::string simd() const;
		int totalPhysicalMemory() const;
		int availablePhysicalMemory() const;
		std::string resolution() const;

		// perf counters
		int nameDatabaseSize() const { return (int) ARL::Name::size(); }
		int nameDatabaseBytes() const { return (int) ARL::Name::approximateMemoryUsage(); }
		double processCores() const;
		double getElapsedTime() const;
		int totalProcessorTime() const;
		int processorTime() const;
		int privateBytes() const;
		int privateWorkingSetBytes() const;
		int GetVirtualBytes() const;
		int GetPageFileBytes() const;
		int GetPageFaultsPerSecond() const;
		long instanceCount() const { return Diagnostics::Countable<Instance>::getCount(); }
		long getPlayerCount() const;
		long getDataModelCount() const;
		long jobCount() const { return Diagnostics::Countable<TaskScheduler::Job>::getCount(); }
		long getCdnSuccessCount() const;
		long getCdnFailureCount() const;
		long getAlternateCdnSuccessCount() const;
		long getAlternateCdnFailureCount() const;
		double getLastCdnFailureTimeSpan() const;
		long getANORRLSuccessCount() const;
		long getANORRLFailureCount() const;
		double getANORRLResponse() const;
		double getCdnResponse() const;
		shared_ptr<const Reflection::Tuple> resetCdnFailureCounts();
	};

	extern const char *const sTaskSchedulerSettings;
	class TaskSchedulerSettings 
		 : public GlobalAdvancedSettingsItem<TaskSchedulerSettings, sTaskSchedulerSettings>
	{
		TaskScheduler::ThreadPoolConfig threadPoolConfig;
	public:
		TaskSchedulerSettings();

		void addDummyJob(bool exclusive, double fps);

		unsigned int threadPoolSize() const { return TaskScheduler::singleton().threadPoolSize(); }
		double threadAffinity() const { return TaskScheduler::singleton().threadAffinity(); }
		double numSleepingJobs() const { return TaskScheduler::singleton().numSleepingJobs(); }
		double numWaitingJobs() const { return TaskScheduler::singleton().numWaitingJobs(); }
		double numRunningJobs() const { return TaskScheduler::singleton().numRunningJobs(); }
		double schedulerRate() const { return TaskScheduler::singleton().schedulerRate(); }
		double schedulerDutyCyclePerThread() const { return TaskScheduler::singleton().getSchedulerDutyCyclePerThread(); }

		bool getIsArbiterThrottled() const { return SimpleThrottlingArbiter::isThrottlingEnabled; }
		void setIsArbiterThrottled(bool value);
		double getThrottledJobSleepTime() const { return TaskScheduler::Job::throttledSleepTime; }
		void setThrottledJobSleepTime(double value);

		TaskScheduler::PriorityMethod getPriorityMethod() const { return TaskScheduler::priorityMethod; }
		void setPriorityMethod(TaskScheduler::PriorityMethod value);

		TaskScheduler::Job::SleepAdjustMethod getSleepAdjustMethod() const { return TaskScheduler::Job::sleepAdjustMethod; }
		void setSleepAdjustMethod(TaskScheduler::Job::SleepAdjustMethod value);

		TaskScheduler::ThreadPoolConfig getThreadPoolConfig() const;
		void setThreadPoolConfig(TaskScheduler::ThreadPoolConfig value);

		void setThreadShare(double timeSlice, int divisor);

		DataModelArbiter::ConcurrencyModel getConcurrencyModel() const { return DataModelArbiter::concurrencyModel; }
		void setConcurrencyModel(DataModelArbiter::ConcurrencyModel value);

	};


} // namespace
