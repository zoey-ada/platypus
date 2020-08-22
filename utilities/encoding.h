#pragma once

#include <string>

namespace Encoding
{
const std::string toUtf8(const std::wstring&);
const std::wstring toUtf16(const std::string&);
}
