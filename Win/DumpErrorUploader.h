#pragma once

#include "ErrorUploader.h"


namespace ARL
{
	class Http;
}

class DumpErrorUploader : public ErrorUploader
{
public:
	DumpErrorUploader(bool backgroundUpload, const std::string& crashCounterNamePrefix);
	void Upload(const std::string& url);
	void InitCrashEvent(const std::string& url, const std::string& crashEventName);

	static void UploadCrashEventFile(struct _EXCEPTION_POINTERS *info = NULL);

	// Really trying to minimize heap allocations on crash event upload
	static boost::scoped_ptr<ARL::Http> crashEventRequest;
	static std::istringstream crashEventData;
	static std::string crashEventResponse;
	static std::string crashCounterNamePrefix;
private:
	static ARL::worker_thread::work_result run(shared_ptr<data> _data);

};

