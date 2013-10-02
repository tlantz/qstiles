#ifndef MOSTER_ASSETPATH_HPP_
#define MOSTER_ASSETPATH_HPP_

#include <sstream>

namespace moster
{
	class assetpath
	{

	private:

		std::stringstream path_;

	public:

		assetpath(const std::string & root = "");

		assetpath & operator<<(const std::string & part);

		std::string str() const;

	};
}

#endif