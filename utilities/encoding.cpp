#include "encoding.h"

#include <codecvt>
#include <locale>

namespace Encoding
{
const std::string toUtf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
	return converter.to_bytes(str);
}

const std::wstring toUtf16(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
	return converter.from_bytes(str);
}
}
