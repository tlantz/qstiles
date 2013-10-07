#ifndef MOSTER_ASSETPATH_HPP_
#define MOSTER_ASSETPATH_HPP_
#include <irrTypes.h>
#include <sstream>

namespace moster
{
	class assetpath
	{

	private:

		std::string buff_;
		std::stringstream path_;

	public:
		
		assetpath(const irr::fschar_t * root);

		assetpath & operator<<(const irr::fschar_t * part);

		const irr::fschar_t * c_str();

	};
}

#endif
