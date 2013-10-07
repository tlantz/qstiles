#include <moster/os.hpp>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <wchar.h>

using namespace irr;

namespace moster { namespace os 
{
	size_t strlen(c8 * str)
	{
		return ::strlen(str);
	}

	const c8 * pathsep()
	{
#ifdef WIN32
		return "\\";
#else
		return "/";
#endif
	}

	template<>
	size_t sprintf(wchar_t * dest, size_t size, const wchar_t * format, ...)
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

	template<>
	size_t sprintf(c8 * dest, size_t size, const c8 * format, ...)
	{
		va_list args;
		va_start(args, format);
#ifdef WIN32
		auto res = ::vsprintf_s(dest, size, format, args);
#else
		auto res = ::vsprintf(dest, size, format, args);
#endif
		va_end(args);
		return res;
	}

	size_t mbstowcs(wchar_t * dest, size_t destsz, const irr::c8 * src, size_t srcsize)
	{
#ifdef WIN32
		size_t res;
		mbstate_t s;
		::mbsrtowcs_s(&res, dest, destsz, &src, srcsize, &s);
		return res;
#else
		return mbstowcs(dest, source, size);
#endif
	}

	size_t wcrtomb(c8 * dest, size_t destsz, const wchar_t * src, size_t srcsize)
	{
#ifdef WIN32
		size_t res;
		mbstate_t s;
		wcsrtombs_s(&res, dest, destsz, &src, srcsize, &s);
		return res;
#else
		return wcsrtombs(dest, source, size);
#endif
	}

} }