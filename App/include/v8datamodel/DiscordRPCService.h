#pragma once

#include "v8tree/Service.h"
#include "v8tree/Instance.h"

namespace ARL {
	class PartInstance;

	extern const char *const sDiscordRPCService;

	class DiscordRPCService 
		: public DescribedCreatable<DiscordRPCService, Instance, sDiscordRPCService, Reflection::ClassDescriptor::INTERNAL_LOCAL>
		, public Service
	{
	private:
		typedef DescribedCreatable<DiscordRPCService, Instance, sDiscordRPCService, Reflection::ClassDescriptor::INTERNAL_LOCAL> Super;
		std::string state;
		std::string details;
	public:
		DiscordRPCService();

		void setState(std::string state);
		void setDetails(std::string details);

		std::string getState() { return state; }
		std::string getDetails() { return details; }

		/*
		int64_t startTimestamp;
		int64_t endTimestamp;
		const char* largeImageKey;   max 32 bytes
		const char* largeImageText;  max 128 bytes
		const char* smallImageKey;   max 32 bytes
		const char* smallImageText;  max 128 bytes
		const char* partyId;         max 128 bytes
		int partySize;
		int partyMax;
		const char* matchSecret;     max 128 bytes
		const char* joinSecret;      max 128 bytes
		const char* spectateSecret;  max 128 bytes
		*/
	};
}