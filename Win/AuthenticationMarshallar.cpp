#include "StdAfx.h"
#include "AuthenticationMarshallar.h"
#include "util/http.h"
#include "rbx/boost.hpp"

#undef min
#undef max

#include "v8datamodel/contentprovider.h"
#include <sstream>
#include <boost/algorithm/string.hpp>

AuthenticationMarshallar::AuthenticationMarshallar(const char* domain)
:domain(domain)
{
}

AuthenticationMarshallar::~AuthenticationMarshallar(void)
{
}

std::string buildUrl(const char* url, const char* ticket)
{
	std::string compound = url;
	if (ticket)
	{
		compound += "?suggest=";
		compound += ticket;
	}

	return compound;
}

std::string AuthenticationMarshallar::Authenticate(const char* url, const char* ticket)
{
	try
	{
		// Post our ticket back to Roblox to suggest a re-authentication
		std::string result;
		{
			ARL::Http http(buildUrl(url, ticket).c_str());
            http.setAuthDomain(domain);
			http.get(result);
		}

		// The http content is the new ticket
		return result;
	}
	catch (std::exception&)
	{
		//Report Error!
		return "";
	}
}

ARL::HttpFuture AuthenticationMarshallar::AuthenticateAsync(const char* url, const char* ticket)
{
    ARL::HttpOptions options;
	options.addHeader(ARL::Http::kARLAuthenticationNegotiation, domain);
	return ARL::HttpAsync::get(buildUrl(url, ticket), options);
}