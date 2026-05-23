#pragma once

#if defined(_WIN32)
#include <windows.h>
#include <wincrypt.h>
#endif

#include <string>

namespace ARL
{
	class Crypt
	{
#if defined (_WIN32)
	HCRYPTPROV context;
	HCRYPTKEY key;
#endif
	public:
		Crypt();
		~Crypt();
		void verifySignatureBase64(std::string message, std::string signatureBase64);
	};
}


