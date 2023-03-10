#if defined(__clang__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wnullability-completeness"
#elif defined(_MSC_VER)
	#pragma warning(push)
// #pragma warning(disable : 4068)
// #pragma warning(disable : 4100)
// #pragma warning(disable : 4127)
// #pragma warning(disable : 4146)
// #pragma warning(disable : 4244)
// #pragma warning(disable : 4267)
// #pragma warning(disable : 4512)
// #pragma warning(disable : 4800)
// #pragma warning(disable : 5054)
#endif

#include "libzip/lib/zip.h"

#if defined(__clang__)
	#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
	#pragma warning(pop)
#endif
