// ACCService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <atomic>
#include <cstdlib>
#include <thread>

#define _CRTDBG_MAP_ALLOC
#ifdef _WIN32
#include <stdlib.h>
#include <crtdbg.h>

#include "conio.h"

#include "logmanager.h"
#include "Util/WinHeap.h"
#endif


#include "gSOAP/generated/ACCServiceSoap.nsmap"
#include "gSOAP/generated/soapACCServiceSoapService.h"
#include "arl/boost.hpp"
#include "Util/StandardOut.h"
#include "arl/TaskScheduler.h"
#include "Util/Statistics.h"
#include "util/Http.h"
#include "v8datamodel/datamodel.h"
#include "OperationalSecurity.h"

#pragma optimize( "", off ) 

static std::atomic<long> requestCount = 0;
extern std::atomic<long> diagCount;
extern std::atomic<long> batchJobCount;
extern std::atomic<long> openJobCount;
extern std::atomic<long> closeJobCount;
extern std::atomic<long> helloWorldCount;
extern std::atomic<long> getVersionCount;
extern std::atomic<long> renewLeaseCount;
extern std::atomic<long> executeCount;
extern std::atomic<long> getExpirationCount;
extern std::atomic<long> getStatusCount;
extern std::atomic<long> getAllJobsCount;
extern std::atomic<long> closeExpiredJobsCount;
extern std::atomic<long> closeAllJobsCount;

#ifdef ARL_TEST_BUILD
extern std::string ACCServiceSettingsKeyOverwrite;
#endif

void process_request(ACCServiceSoapService* service)
{
	requestCount++;
	try
	{
		service->serve();
		delete service;
	}
	catch (...)
	{
		ARLCRASH();
	}
	requestCount--;
}

static void StringCrash(const char* s)
{
	//char str[256];
	//strncpy_s(str, 256, s, 256);
	ARLCRASH();
}

template <class Soap>
class ExceptionAwareSoap : public Soap
{
public:
	virtual	int dispatch()
	{
		try
		{
			//printf("%s\n", action); 
			return Soap::dispatch();
		}
		catch (std::exception& e)
		{
			return soap_receiver_fault(this, e.what(), NULL); // return fault to sender 
		}
		catch (std::string& s)
		{
			StringCrash(s.c_str());
			return soap_receiver_fault(this, s.c_str(), NULL); // return fault to sender 
		}
		catch (...)
		{
			ARLCRASH();
			return soap_receiver_fault(this, "Unexpected C++ exception type", NULL); // return fault to sender 
		}
	}
	virtual	ACCServiceSoapService *copy()
	{
		ExceptionAwareSoap<Soap> *dup = new ExceptionAwareSoap<Soap>();
		soap_copy_context(dup, this);
		return dup;	
	}
};


ExceptionAwareSoap<ACCServiceSoapService> service; 




void start_CWebService(const std::string& contentpath, bool crashUploaderOnly);

static void startupACC(int port, const std::string &contentpath, bool crashUploaderOnly)
{
	printf("Service starting...\n"); 

	start_CWebService(contentpath, crashUploaderOnly);

	//service.send_timeout = 60; // 60 seconds 
	//service.recv_timeout = 60; // 60 seconds 
	service.accept_timeout = 1; // server stops after 1 second
	//soap.max_keep_alive = 100; // max keep-alive sequence 
	SOAP_SOCKET m = service.bind(NULL, port, 100); 
	if (!soap_valid_socket(m)) 
		throw std::runtime_error(*soap_faultstring(&service)); 

	char buffer[64];
	sprintf_s(buffer, 64, "Service Started on port %d", port); 
	ARL::StandardOut::singleton()->print(ARL::MESSAGE_SENSITIVE, buffer);
}

void process_request_func(ACCServiceSoapService* param)
{
	process_request(param);
}

static void stepACC()
{
	if (requestCount>100)
		throw std::runtime_error(ARL::format("%d outstanding requests, execute=%d, openJob=%d, batchJob=%d, diag=%d, getVersion=%d, renewLease=%d, getAllJobs=%d, getStatusCount=%d, closeExpiredJobs=%d, closeJob=%d, helloWorld=%d, getExpiration=%d, closeAllJobs=%d", 
								 requestCount, executeCount, openJobCount, batchJobCount, diagCount, getVersionCount, renewLeaseCount, getAllJobsCount, getStatusCount, closeExpiredJobsCount, closeJobCount, helloWorldCount, getExpirationCount, closeAllJobsCount));

	SOAP_SOCKET s = service.accept(); 
	if (!soap_valid_socket(s)) 
	{ 
		if (service.errnum)
			throw std::runtime_error(*soap_faultstring(&service)); 
		return;
	} 

	ACCServiceSoapService* copy = service.copy(); // make a safe copy 
	if (!copy) 
		throw std::runtime_error(*soap_faultstring(&service)); 

	try
	{
		std::thread(process_request_func, copy).detach();
	}
	catch (const std::system_error& e)
	{
		fprintf(stderr, "Thread creation failed: %s\n", e.what());
	}
}

void stop_CWebService();

static void shutdownACC()
{
	printf("Service shutting down...\n"); 
	stop_CWebService();
}


static int parsePort(int argc, char* argv[])
{
	int port = 64989;
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (!arg.empty() && (arg[0] == '/' || arg[0] == '-'))
			continue;
		port = std::atoi(arg.c_str());
	}
	return port;
}

static std::string parseContent(int argc, char* argv[])
{
	const std::string tag = "-Content:";
	std::string result = parse(tag, argc, argv);
	return result.empty() ? "content/" : result;
}

static std::string parseBaseUrl(int argc, char* argv[])
{
	const std::string tag = "-BaseUrl:";
	std::string result = parse(tag, argc, argv);
	return result.empty() ? "" : result;
}

static std::string parse(const std::string& tag, int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];

		if (!arg.empty() && arg[0] == '/')
			arg[0] = '-';

		if (arg.size() >= tag.size() &&
			std::equal(tag.begin(), tag.end(), arg.begin(),
				[](char a, char b)
				{ return std::tolower(a) == std::tolower(b); }))
		{
			return arg.substr(tag.size());
		}
	}

	return "";
}

static int parsePlaceId(int argc, char* argv[])
{
	std::string result = parse("-PlaceId:", argc, argv);
	if (result.empty())
		return -1;
	return std::atoi(result.c_str());
}

#ifdef ARL_TEST_BUILD
static LPCTSTR parseMD5(int argc, _TCHAR* argv[])
{
	const _TCHAR szTag[] = _T("-Md5:");
	return parse(szTag, sizeof(szTag), argc, argv);
}

static LPCTSTR parseSettingsKey(int argc, _TCHAR* argv[])
{
	const _TCHAR szTag[] = _T("-SettingsKey:");
	return parse(szTag, sizeof(szTag), argc, argv);
}
#endif


namespace ARL {
	extern bool nameThreads;
}

static boost::mutex keyLockMutex;

void ReadAccessKey()
{
	if (ARL::Http::accessKey.empty())
	{
		const char* envKey = std::getenv("ACC_ACCESS_KEY");
		if (envKey && *envKey)
		{
			ARL::Http::accessKey = std::string(envKey);
			ARL::StandardOut::singleton()->printf(
				ARL::MESSAGE_SENSITIVE, "Got access key: %s", ARL::Http::accessKey.c_str());
		}
		else
		{
			ARL::StandardOut::singleton()->printf(
				ARL::MESSAGE_WARNING, "No access key has been set!");
		}
	}
	else
	{
		ARL::StandardOut::singleton()->printf(
			ARL::MESSAGE_SENSITIVE, "Current access key: %s", ARL::Http::accessKey.c_str());
	}
}

static std::string parseBaseUrl(int argc, char* argv[])
{
	const std::string tag = "-BaseUrl:";
	std::string result = parse(tag, argc, argv);
	return result.empty() ? "" : result;
}

class PrintfLogger
{
	arl::signals::scoped_connection messageConnection;
	arl::spin_mutex mutex;
public:
	PrintfLogger()
	{
		messageConnection = ARL::StandardOut::singleton()->messageOut.connect(boost::bind(&PrintfLogger::onMessage, this, _1));
	}
protected:
	void onMessage(const ARL::StandardOutMessage& message)
	{
		arl::spin_mutex::scoped_lock lock(mutex);

		const char* colorCode;

		switch (message.type)
		{
		case ARL::MESSAGE_OUTPUT:
			colorCode = "\033[1;34m";
			break;
		case ARL::MESSAGE_INFO:
			colorCode = "\033[0m";
			break;
		case ARL::MESSAGE_WARNING:
			colorCode = "\033[1;33m";
			break;
		case ARL::MESSAGE_ERROR:
			colorCode = "\033[1;31m";
			break;
		default:
			colorCode = "\033[0m";
			break;
		}

		std::cout << colorCode << message.message << "\033[0m" << std::endl;
	}
};

int main(int argc, char* argv[])
{
	static boost::scoped_ptr<PrintfLogger> standardOutLog(new PrintfLogger());
	ReadAccessKey();

	std::string baseUrl = parseBaseUrl(argc, argv);
	SetBaseURL(baseUrl);

	try
	{
#ifdef ARL_TEST_BUILD
		ARL::DataModel::hash = parseMD5(argc, argv);
		ACCServiceSettingsKeyOverwrite = parseSettingsKey(argc, argv);
#endif
		startupACC(parsePort(argc, argv), parseContent(argc, argv), false);

		int placeId = parsePlaceId(argc, argv);
		if (placeId > -1)
		{
			std::stringstream buffer;
			std::ifstream gameServerFile("gameserver.txt", std::ios::in);
			if (gameServerFile.is_open())
			{
				buffer << gameServerFile.rdbuf();
				gameServerFile.close();
			}

			buffer << "start(" << placeId << ", " << 53640 << ", '" << GetBaseURL() << "')";
			std::string script = buffer.str();

			ACCServiceSoapService* copy = service.copy(); // make a safe copy
			if (!copy)
				throw std::runtime_error(*soap_faultstring(&service));

			ns1__Job job;
			job.id = "Test";
			job.expirationInSeconds = 600;

			ns1__ScriptExecution se;
			static std::string scriptName = "Start Server";
			se.name = &scriptName;
			se.script = &script;

			_ns1__OpenJob openJob;
			_ns1__OpenJobResponse response;
			openJob.script = &se;
			openJob.job = &job;

			copy->OpenJob(&openJob, &response);
		}

		while (true) // TODO: add some form of graceful shutdown like a ctrl c
		{
			stepACC();
		}
		shutdownACC();
	}
	catch (std::exception& e)
	{
		ARL::StandardOut::singleton()->print(ARL::MESSAGE_ERROR, e);
	}
	ARL::clearLuaReadOnly();
}

#pragma optimize( "", on )
