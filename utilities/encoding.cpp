#include "encoding.hpp"

#include <utf8.h>

#include <codecvt>
#include <locale>

namespace Encoding
{
const std::string toUtf8(const std::u16string& str)
{
	return utf8::utf16to8(str);
}

const std::u16string toUtf16(const std::string& str)
{
	return utf8::utf8to16(str);
}

#ifdef _WIN32
	const std::string fromWindowsString(const std::wstring& str)
	{
		std::u16string wstr(str.begin(), str.end());
		return toUtf8(wstr);
	}

	const std::wstring toWindowsString(const std::string& str)
	{
		std::u16string wstr = utf8::utf8to16(str);
		return std::wstring(wstr.begin(), wstr.end());
	}
#endif

}
