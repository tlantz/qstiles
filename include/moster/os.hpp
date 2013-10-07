#ifndef MOSTER_OS_HPP_
#define MOSTER_OS_HPP_

#include <irrTypes.h>

namespace moster { namespace os
{	
	size_t mbstowcs(wchar_t * dest, size_t destsz, const irr::c8 * src, size_t srcsize);

	size_t wcrtomb(irr::c8 * dest, size_t destsz, const wchar_t * src, size_t srcsize);

	const irr::c8 * pathsep();

	size_t strlen(irr::c8 * str);

	template <typename c_t>
	size_t sprintf(c_t * dest, size_t size, const c_t * format, ...);

} }

#endif