#include <string>
#include <stdexcept>
#include <cstdint>

namespace ARL {

std::string utf8_decode(const std::string& str) {
    return str;
}

std::string utf8_encode(const std::wstring& wstr) {
    std::string result;

    for (wchar_t wc : wstr) {
        uint32_t ch = static_cast<uint32_t>(wc);

        if (ch <= 0x7F) {
            result += static_cast<char>(ch);
        } else if (ch <= 0x7FF) {
            result += static_cast<char>(0xC0 | ((ch >> 6) & 0x1F));
            result += static_cast<char>(0x80 | (ch & 0x3F));
        } else if (ch >= 0xD800 && ch <= 0xDBFF) {
            throw std::runtime_error("Surrogate pairs not supported in this simple encoder");
        } else if (ch <= 0xFFFF) {
            result += static_cast<char>(0xE0 | ((ch >> 12) & 0x0F));
            result += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (ch & 0x3F));
        } else {
            ch -= 0x10000;
            wchar_t high = static_cast<wchar_t>((ch >> 10) + 0xD800);
            wchar_t low = static_cast<wchar_t>((ch & 0x3FF) + 0xDC00);
            throw std::runtime_error("Encoding surrogate pairs not supported in this minimal example");
        }
    }

    return result;
}

}
