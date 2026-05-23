#pragma once

#include <string>

#include <boost/filesystem.hpp>

#include "arl/atomic.h"
#include "arl/CEvent.h"
#include "arl/RunningAverage.h"
#include "arl/rbxTime.h"
#include "util/HttpAux.h"

DYNAMIC_FASTFLAG(UseAssetTypeHeader)

namespace ARL
{
    namespace HttpCache
    {
    enum Policy
    {
        // No caching by default.
        PolicyDefault,

        // Cache based on the final URL (after all 302s).
        PolicyFinalRedirect,
    };
    } // namespace HttpCache

    class mutex;

	class http_status_error:
		public std::runtime_error
	{
	public:
		int statusCode;
		http_status_error(int statusCode);
		http_status_error(int statusCode, const std::string& message);
    };

	class Http
	{
	public:
        typedef enum { Uninitialized=-1 ,WinInet=0, WinHttp=1, XboxHttp=2 } API;

        enum CookieSharingPolicy
        {
            CookieSharingUndefined                      = 0x0,
            CookieSharingMultipleProcessesRead          = 0x1,
            CookieSharingMultipleProcessesWrite         = 0x2,
            CookieSharingSingleProcessMultipleThreads   = 0x4,
        };

        inline friend CookieSharingPolicy operator|(CookieSharingPolicy a, CookieSharingPolicy b)
        {
            return static_cast<CookieSharingPolicy>(static_cast<int>(a) | static_cast<int>(b));
        }

        inline friend CookieSharingPolicy operator|=(CookieSharingPolicy a, CookieSharingPolicy b)
        {
            return static_cast<CookieSharingPolicy>(static_cast<int>(a) | static_cast<int>(b));
        }

		static std::string accessKey;
		static std::string gameSessionID; // additional header to be sent in POST requests to anorrl
		static std::string gameID;
		static std::string placeID;
		static std::string requester;
        static std::string rbxUserAgent;
        
		static int playerCount;

        static bool useDefaultTimeouts;

        // Defined in Utilities.cpp.
		static const std::string kGameSessionHeaderKey;
		static const std::string kGameIdHeaderKey;
		static const std::string kPlaceIdHeaderKey;
		static const std::string kRequesterHeaderKey;
		static const std::string kPlayerCountHeaderKey;
		static const std::string kAccessHeaderKey;
		static const std::string kAssetTypeKey;
        static const std::string kARLAuthenticationNegotiation;
		static const std::string kContentTypeDefaultUnspecified;
		static const std::string kContentTypeUrlEncoded;
		static const std::string kContentTypeApplicationJson;
		static const std::string kContentTypeApplicationXml;
		static const std::string kContentTypeTextPlain;
		static const std::string kContentTypeTextXml;

	private:
        static API defaultApi;
        static CookieSharingPolicy cookieSharingPolicy;

		std::string alternateUrl;	// Used if a CDN fails to deliver and we can try an alternate URL for gets

        HttpCache::Policy cachePolicy;

        int connectTimeoutMillis;
        int responseTimeoutMillis;
        int sendTimeoutMillis;
        int dataSendTimeoutMillis;

        API instanceApi;

		static ARL::mutex *anorrlResponseLock;
		static ARL::mutex *cdnResponseLock;
		static std::string lastCsrfToken;
		static boost::mutex lastCsrfTokenMutex;

		class MutexGuard
		{
		public:
			MutexGuard();
			~MutexGuard();
		};

        static MutexGuard lockGuard;

        void init();
	public:
		static void init(API api, CookieSharingPolicy cookieSharingPolicy);
        static void SetUseStatistics(bool value);
        static void SetUseCurl(bool value);
        static arl::atomic<int> cdnSuccessCount;
        static arl::atomic<int> cdnFailureCount;
        static arl::atomic<int> alternateCdnSuccessCount;
        static arl::atomic<int> alternateCdnFailureCount;
		static double lastCdnFailureTimeSpan;
        static arl::atomic<int> anorrlSuccessCount;
        static arl::atomic<int> anorrlFailureCount;

		static WindowAverage<double, double> anorrlResponse;
		static WindowAverage<double, double> cdnResponse;

		static ARL::mutex *getANORRLResponseLock();
		static ARL::mutex *getCdnResponseLock();

        std::string url;
		Http():instanceApi(defaultApi),url("") { init(); }
		Http(const char* url):instanceApi(defaultApi),url(url) { init(); }
		Http(const char* url, API api):instanceApi(api),url(url) { init(); }
		Http(const std::string& url):instanceApi(defaultApi),url(url) { init(); }
        Http(const std::string& url, API api):instanceApi(api),url(url) { init(); }

        bool recordStatistics;
		bool shouldRetry;
		HttpAux::AdditionalHeaders additionalHeaders;
		bool doNotUseCachedResponse;
		std::string authDomainUrl;
        void setAuthDomain(std::string domain)
        {
            authDomainUrl = domain;
            additionalHeaders[kARLAuthenticationNegotiation] = domain;
        }
		void setExpectedAssetType(const std::string& type)
		{
			if (DFFlag::UseAssetTypeHeader && !type.empty())
				additionalHeaders[kAssetTypeKey] = type;
		}

        static void setCookiesForDomain(const std::string& domain, const std::string& cookies);
		static void getCookiesForDomain(const std::string& domain, std::string& cookies);
		
        void setResponseTimeout(int timeout) { responseTimeoutMillis = timeout; }
        void setSendTimeout(int timeout) { sendTimeoutMillis = timeout; }
        void setDataSendTimeout(int timeout) { dataSendTimeoutMillis = timeout; }
        void setConnectionTimeout(int timeout) { connectTimeoutMillis = timeout; }
        void setCachePolicy(const HttpCache::Policy policy) { cachePolicy = policy; }

		// Async
		void post(const std::string& input, const std::string& contentType, bool compress, boost::function<void(std::string*, std::exception*)> handler, bool externalRequest = false);
		void post(boost::shared_ptr<std::istream> input, const std::string& contentType, bool compress, boost::function<void(std::string *, std::exception*)> handler, bool externalRequest = false);
		void get(boost::function<void(std::string*, std::exception*)> handler, bool allowExternal = false);

		// Sync
		void post(std::istream& input, const std::string& contentType, bool compress, std::string& response, bool externalRequest = false);
		void get(std::string& response, bool allowExternal = false);

		static bool isExternalRequest(const char* url);
		static bool trustCheck(const char* url, bool allowExternal = false);
		static bool trustCheckBrowser(const char* url);
		static bool isScript(const char* url);
		static bool isANORRLSite(const char* url);
        static bool isStrictlyANORRLSite(const char* url);
		
		// Utility
		static std::string urlEncode(const std::string& s);
		// urlDecode is only tested to work on strings produced from urlEncode
		static std::string urlDecode(const std::string& fragment);

		void applyAdditionalHeaders(ARL::HttpAux::AdditionalHeaders& outHeaders);

	private:
        void httpGetPost(bool isPost, std::istream& dataStream, const std::string& contentType, bool compressData, const HttpAux::AdditionalHeaders& additionalHeaders, bool allowExternal, std::string& response, bool forceNativeHttp = false);
#if defined(_WIN32)
        void httpGetPostWinInet(bool isPost, std::istream& dataStream, const std::string& contentType, bool compressData, const HttpAux::AdditionalHeaders& additionalHeaders, bool allowExternal, std::string& response);
        void httpGetPostWinHttp(bool isPost, std::istream& dataStream, const std::string& contentType, bool compressData, const HttpAux::AdditionalHeaders& additionalHeaders, bool allowExternal, std::string& response);
#elif defined(__APPLE__)
        void httpGetPostImpl(bool isPost, std::istream& dataStream, const std::string& contentType, bool compressData, const HttpAux::AdditionalHeaders& additionalHeaders, bool allowExternal, std::string& response);
#endif
		bool doHttpGetPostWithNativeFallbackForReporting(bool isPost, std::istream& dataStream, const std::string& contentType, bool compressData, const HttpAux::AdditionalHeaders& additionalHeaders, bool allowExternal, std::string& response);

		void ThrowIfFailure(bool success, const char* message);

#ifdef _WIN32
		static void setCookiesForDomainWinInet(const std::string& domain, const std::string& cookies);
#endif
	public:
#ifdef _WIN32
		void onWinHttpRedirect(unsigned long dwInternetStatus, std::string redirectUrl);
#endif

		static void ThrowIfFailure(bool success, const char* url, const char* message);	

#if defined(_WIN32)
		static void ThrowLastError(int error, const char* url, const char* message);
#endif

		static void setProxy(const std::string& host, long port = 0);

		// These methods are not safe to be called from static initializers, because they rely on
		// static member variables (which don't have order guarantees wrt other static initializers).
		static std::string getLastCsrfToken();
		static void setLastCsrfToken(const std::string& newToken);
    };
}
