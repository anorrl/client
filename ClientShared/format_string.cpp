#include "format_string.h"
#include <sstream>

std::string convert_w2s(const std::wstring &str)
{
	std::string result;
	std::copy(str.begin(), str.end(), std::back_inserter(result));
	return result;
}
  
std::wstring convert_s2w(const std::string &str)
{
	std::wstring result;
	std::copy(str.begin(), str.end(), std::back_inserter(result));
	return result;
}

std::string vformat(const char *fmt, va_list argPtr) {
    // We draw the line at a 1MB string.
    const int maxSize = 1000000;

    // If the string is less than 161 characters,
    // allocate it on the stack because this saves
    // the malloc/free time.
    const int bufSize = 161;
	char stackBuffer[bufSize];

    // Using va_copy to safely use the argPtr again after vsnprintf for determining the buffer size
	va_list argPtrCopy;
	va_copy(argPtrCopy, argPtr);
	int requiredSize = vsnprintf(nullptr, 0, fmt, argPtrCopy) + 1; // +1 for the null-terminator
	va_end(argPtrCopy);

    if (requiredSize > bufSize) {

        // Now use the heap.
        char* heapBuffer = NULL;

        if (requiredSize < maxSize) {

            heapBuffer = (char*)malloc(maxSize + 1);
			if (!heapBuffer)
				throw std::bad_alloc();
            vsnprintf(heapBuffer, maxSize, fmt, argPtr);
            heapBuffer[maxSize] = '\0';
        } else {
            heapBuffer = (char*)malloc(requiredSize);
			vsnprintf(heapBuffer, requiredSize-1, fmt, argPtr);
			heapBuffer[requiredSize-1] = '\0';
        }

        std::string formattedString(heapBuffer);
        free(heapBuffer);
        return formattedString;
    } else {
		vsnprintf(stackBuffer, bufSize-1, fmt, argPtr);
		stackBuffer[bufSize-1] = '\0';
        return std::string(stackBuffer);
    }
}

std::string format_string(const char* fmt,...) {
    va_list argList;
    va_start(argList,fmt);
    std::string result = vformat(fmt, argList);
    va_end(argList);
	
    return result;
}

std::vector<std::string> splitOn(
    const std::string& str,
    const char&        delimeter,
    const bool         trimEmpty )
{
    std::vector<std::string> tokens;

    std::size_t begin = 0;
    std::size_t end = str.find(delimeter);

    if ( end != std::string::npos )
    {
        while ( end != std::string::npos )
        {
            std::string tmp = str.substr( begin, end - begin );
            if ( !tmp.empty() || !trimEmpty )
                tokens.push_back( tmp );

            begin = end + 1;
            end = str.find( delimeter, begin );

            if ( end == std::string::npos )
            {
                tmp = str.substr( begin, str.length() - begin );
                if ( !tmp.empty() || !trimEmpty )
                    tokens.push_back( tmp );
            }
        }
    }
    else if ( !str.empty() || !trimEmpty )
        tokens.push_back( str );

    return tokens;
}

std::vector<std::wstring> splitOn(
    const std::wstring& str,
    const wchar_t&      delimeter,
    const bool          trimEmpty )
{
    std::vector<std::wstring> tokens;

    std::size_t begin = 0;
    std::size_t end = str.find(delimeter);

    if ( end != std::wstring::npos )
    {
        while ( end != std::wstring::npos )
        {
            std::wstring tmp = str.substr( begin, end - begin );
            if ( !tmp.empty() || !trimEmpty )
                tokens.push_back( tmp );

            begin = end + 1;
            end = str.find( delimeter, begin );

            if ( end == std::wstring::npos )
            {
                tmp = str.substr( begin, str.length() - begin );
                if ( !tmp.empty() || !trimEmpty )
                    tokens.push_back( tmp );
            }
        }
    }
    else if ( !str.empty() || !trimEmpty )
        tokens.push_back( str );

    return tokens;
}

