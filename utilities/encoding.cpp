#include "encoding.hpp"

#include <codecvt>
#include <locale>

#include <utf8.h>

namespace Encoding
{

const std::string toUtf8(const std::u32string& str)
{
	return utf8::utf32to8(str);
}

const std::u32string toUtf32(const std::string& str)
{
	return utf8::utf8to32(str);
}

#ifdef _WIN32

const std::string fromWindowsString(const std::wstring& str)
{
	std::u16string wstr(str.begin(), str.end());
	return utf8::utf16to8(wstr);
}

const std::wstring toWindowsString(const std::string& str)
{
	std::u16string wstr = utf8::utf8to16(str);
	return std::wstring(wstr.begin(), wstr.end());
}

#endif

}
