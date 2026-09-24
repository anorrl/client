#include "format_string.h"
#ifdef __linux__
#include <cstdarg>
#endif
#include <sstream>

std::string vformat(const char* fmt, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    int len = std::vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) throw std::runtime_error("String format error");

    std::vector<char> buffer(len + 1);
    std::vsnprintf(buffer.data(), buffer.size(), fmt, args);

    return std::string(buffer.data(), len);
}

std::string format_string(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::string result = vformat(fmt, args);
    va_end(args);
    return result;
}

std::wstring vformat(const wchar_t* fmt, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    int len = std::vswprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) throw std::runtime_error("Wide string format error");

    std::vector<wchar_t> buffer(len + 1);
    std::vswprintf(buffer.data(), buffer.size(), fmt, args);

    return std::wstring(buffer.data(), len);
}

std::wstring format_string(const wchar_t* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::wstring result = vformat(fmt, args);
    va_end(args);
    return result;
}

std::string convert_w2s(const std::wstring& str) {
    std::string result;
    result.reserve(str.size());
    for (wchar_t wc : str) {
        result.push_back(static_cast<char>(wc & 0xFF)); // crude ANSI-ish fallback
    }
    return result;
}

std::wstring convert_s2w(const std::string& str) {
    std::wstring result;
    result.reserve(str.size());
    for (char c : str) {
        result.push_back(static_cast<unsigned char>(c));
    }
    return result;
}

std::vector<std::string> splitOn(const std::string& str, char delimiter, bool trimEmpty) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        if (!item.empty() || !trimEmpty) {
            tokens.push_back(item);
        }
    }
    return tokens;
}

std::vector<std::wstring> splitOn(const std::wstring& str, wchar_t delimiter, bool trimEmpty) {
    std::wstringstream ss(str);
    std::wstring item;
    std::vector<std::wstring> tokens;
    while (std::getline(ss, item, delimiter)) {
        if (!item.empty() || !trimEmpty) {
            tokens.push_back(item);
        }
    }
    return tokens;
}
