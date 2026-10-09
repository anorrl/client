#include "ARLFormat.h"
#include "ARLPlatform.h"
#include "FastLog.h"

#include <math.h>
#include "boost/scoped_array.hpp"

#define NEWLINE "\r\n"

#ifdef _MSC_VER
// disable: "C++ exception handler used"
#   pragma warning (push)
#   pragma warning (disable : 4530)
#endif // _MSC_VER

// If your platform does not have vsnprintf, you can find a
// implementation at http://www.ijs.si/software/snprintf/

namespace ARL {

	std::runtime_error runtime_error(const char* fmt,...) {
		va_list argList;
		va_start(argList,fmt);
		std::string result = vformat(fmt, argList);
		va_end(argList);

		return std::runtime_error(result);
	}

	std::string format(const char* fmt,...) {
		va_list argList;
		va_start(argList,fmt);
		std::string result = vformat(fmt, argList);
		va_end(argList);

		return result;
	}

	std::string vformat(const char *fmt, va_list argPtr) {

		if (!fmt)
			return "";

		// We draw the line at a 1MB string.
		const int maxSize = 1000000;

		// If the string is less than 161 characters,
		// allocate it on the stack because this saves
		// the malloc/free time.
		const int stackBufferSize = 161;

#ifdef _WIN32
		int actualSize = _vscprintf(fmt, argPtr);
		if (actualSize < stackBufferSize) {
			char stackBuffer[stackBufferSize];
			vsnprintf_s(stackBuffer, stackBufferSize, stackBufferSize, fmt, argPtr);
			return std::string(stackBuffer);
		} 
		else 
		{
			// Use the heap.
			boost::scoped_array<char> heapBuffer;

			if (actualSize > maxSize)
				actualSize = maxSize;

			heapBuffer.reset(new char[actualSize+1]);
			vsnprintf_s(heapBuffer.get(), actualSize+1, actualSize, fmt, argPtr);            

			std::string result(heapBuffer.get());
			return result;
		}
#else 
		char stackBuffer[stackBufferSize];
		char* heapBuffer = nullptr;
		std::string formattedString;

		// Using va_copy to safely use the argPtr again after vsnprintf for determining the buffer size
		va_list argPtrCopy;
		va_copy(argPtrCopy, argPtr);
		int requiredSize = vsnprintf(nullptr, 0, fmt, argPtrCopy) + 1; // +1 for the null-terminator
		va_end(argPtrCopy);

		if (requiredSize > stackBufferSize)
		{
			int bufferSize = requiredSize < maxSize ? maxSize : requiredSize;
			heapBuffer = static_cast<char*>(malloc(bufferSize));
			if (!heapBuffer)
				throw std::bad_alloc();

			vsnprintf(heapBuffer, bufferSize, fmt, argPtr);
			heapBuffer[bufferSize] = '\0';
			formattedString.assign(heapBuffer);
			free(heapBuffer);
		}
		else
		{
			vsnprintf(stackBuffer, stackBufferSize, fmt, argPtr);
			stackBuffer[stackBufferSize] = '\0';
			formattedString.assign(stackBuffer);
		}

		return formattedString;
#endif

	}
} // namespace


#ifdef _MSC_VER
#   pragma warning (pop)
#endif

#undef NEWLINE
