#include <moster/diagnostics.hpp>
#include <moster/os.hpp>

using namespace irr;
using namespace irr::core;

namespace moster { namespace diag {

	size_t strfvec(c8 * dest, size_t maxsize, const vector3df & vector)
	{
		return os::sprintf<c8>(dest, maxsize, "(%f,%f,%f)",
			vector.X, vector.Y, vector.Z);
	}

} }