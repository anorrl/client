#pragma once

#include "v8tree/Instance.h"
#include "v8tree/Service.h"

namespace ARL
{
	extern const char* const sCookiesService;
	class CookiesService
		: public DescribedNonCreatable<CookiesService, Instance, sCookiesService>
		, public Service
	{
	private:
		typedef DescribedNonCreatable<CookiesService, Instance, sCookiesService> Super;
		std::string path;
	public:
		CookiesService();

		void SetValue(std::string key, std::string value);
		std::string GetValue(std::string key);
		void DeleteValue(std::string key);
	};
}