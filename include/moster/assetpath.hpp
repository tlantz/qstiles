#ifndef MOSTER_ASSETPATH_HPP_
#define MOSTER_ASSETPATH_HPP_
#include <moster/os.hpp>
#include <sstream>

namespace moster
{
	class assetpath
	{

	private:

		os::stringstream path_;

	public:
		
		assetpath(const os::string & root = L"");

		assetpath & operator<<(const os::string & part);

		os::string str() const;

	};
}

#endif
