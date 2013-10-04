#include <moster/os.hpp>
#include <cstdio>
#include <cstdarg>

namespace moster { namespace os 
{
	size_t strlen(const char * str)
	{
		return ::strlen(str);
	}

	size_t wsprintf(osstr dest, size_t size, const osstr format, ...)
	{
		va_list args;
		va_start(args, format);
#ifdef WIN32
		return ::vswprintf_s(dest, size, format, args);
#else
		return ::vsnprintf(dest, size, format, args)
#endif
		va_end(args);
	}

	size_t mbstowcs(osstr dest, size_t destsz, const char * src, size_t srcsize)
	{
#ifdef WIN32
		size_t res;
		mbstowcs_s(&res, dest, destsz, src, srcsize);
		return res;
#else
		return mbstowcs(dest, source, size);
#endif
	}

} }