#pragma once

#include <string>
#include "Util/HttpAsync.h"

class SimpleJSON;

#ifdef ARL_TEST_BUILD
void SetDefaultFilePath(const std::string &path);
const std::string &GetDefaultFilePath();
#endif

void SetBaseURL(const std::string& baseUrl);
const std::string& GetBaseURL();

void ReportStatisticWithMessage(const std::string& baseUrl, const std::string& id, 
					 const std::string& simpleMessage,
					 const char* secondaryFilterName = NULL, const char* secondaryFilterValue = NULL);

void ReportStatistic(const std::string& baseUrl, const std::string& id,
					 const std::string& primaryFilterName, const std::string& primaryFilterValue,
					 const std::string& secondaryFilterName, const std::string& secondaryFilterValue);

void ReportStatisticPost(const std::string& baseUrl, const std::string& id, const std::string& postData,
					     const char* secondaryFilterName, const char* secondaryFilterValue);


std::string UploadLogFile(const std::string& baseUrl, const std::string& data);

bool FetchLocalClientSettingsData(const char* group, SimpleJSON* dest);
void LoadClientSettingsFromString(const char* group, const std::string& settingsData, SimpleJSON* dest);
bool FetchClientSettingsData(const char* group, const char* apiKey, SimpleJSON* dest);
void FetchClientSettingsData(const char* group, const char* apiKey, std::string* dest);
ARL::HttpFuture FetchClientSettingsDataAsync(const char* group, const char* apiKey);
ARL::HttpFuture FetchABTestDataAsync(const std::string& url);
std::string LoadABTestFromString(const std::string& responseData);