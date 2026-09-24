/*
 *  LogProvider.cpp
 *  RobloxStudio
 *
 *  Created by Ganesh Agrawal on 12/12/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

// Roblox Headers
#include "util/FileSystem.h"
#include "util/Guid.h"
#include "arl/boost.hpp"

// Roblox Studio Headers
#include "LogProvider.h"

#ifdef __APPLE__
#include "BreakpadCrashReporter.h"
#endif


ARL::mutex LogProvider::fastLogChannelsLock;
LogProvider* LogProvider::mainLogManager;

LogProvider::LogProvider()
{
	logDir = ARL::FileSystem::getLogsDirectory();
    ARL::Guid::generateARLGUID(logGuid);
	mainLogManager = this;
	FLog::SetExternalLogFunc(LogProvider::FastLogMessage);
}

void LogProvider::FastLogMessage(FLog::Channel id, const char* message) {

	ARL::mutex::scoped_lock lock(fastLogChannelsLock);

	if(mainLogManager)
	{
		if(id >= mainLogManager->fastLogChannels.size())
			mainLogManager->fastLogChannels.resize(id+1, NULL);

		if(mainLogManager->fastLogChannels[id] == NULL)
		{
			char temp[20];
			snprintf(temp, 19, "log_%s_%u.txt", mainLogManager->logGuid.substr(3,6).c_str(), id);
			temp[19] = 0;
            
            boost::filesystem::path logFile = mainLogManager->logDir / temp;

			mainLogManager->fastLogChannels[id] = new ARL::Log(logFile.c_str(), "Log Channel");
#ifndef _DEBUG
			CrashReporter::addBreakpadLog(logFile.c_str());
#endif
		}

		mainLogManager->fastLogChannels[id]->writeEntry(ARL::Log::Information, message);
	}
}
	
LogProvider::~LogProvider()
{
}

ARL::Log* LogProvider::provideLog()
{
	ARL::Log* result = log.get();
	if (!result)
	{
		std::string name = ARL::get_thread_name();
        boost::filesystem::path logFile = logDir / ("log_" + logGuid.substr(3,6) +".txt");
		
		result = new ARL::Log(logFile.c_str(), name.c_str());
		log.reset(result);
#ifndef _DEBUG
		CrashReporter::addBreakpadLog(logFile.c_str());
#endif
	}
	return result;
}