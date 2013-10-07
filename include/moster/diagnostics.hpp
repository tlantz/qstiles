#ifndef MOSTER_DIAGNOSTICS_HPP_
#define MOSTER_DIAGNOSTICS_HPP_

#include <irrTypes.h>
#include <vector3d.h>

namespace moster { namespace diag
{
	size_t strfvec(irr::c8 * dest, size_t maxsize, const irr::core::vector3df & vector);

	template <size_t sz>
	const irr::c8 * strfvec(const irr::core::vector3df & vector)
	{
		static char buff[sz];
		strfvec(buff, sz, vector);
		return buff;
	}

} }

#endif