#pragma once

#include <string>

namespace Encoding
{

const std::string toUtf8(const std::u32string&);
const std::u32string toUtf32(const std::string&);

#ifdef _WIN32
const std::string fromWindowsString(const std::wstring&);
const std::wstring toWindowsString(const std::string&);
#endif

}
