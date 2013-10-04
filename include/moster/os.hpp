#ifndef MOSTER_OS_HPP_
#define MOSTER_OS_HPP_

#include <string>

namespace moster { namespace os
{
	typedef wchar_t oschar;
	typedef wchar_t * osstr;
	typedef std::wstring string;
	typedef std::wstringstream stringstream;

	const char * pathsep();

	const osstr wpathsep();
	
	size_t mbstowcs(osstr dest, size_t destsz, const char * src, size_t srcsize);

	size_t strlen(const char * str);

	size_t wsprintf(osstr dest, size_t size, const osstr format, ...);

} }

#endif