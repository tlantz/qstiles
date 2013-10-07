#include <moster/assetpath.hpp>
#include <moster/os.hpp>

using namespace irr;

namespace moster
{
	assetpath::assetpath(const fschar_t * root)
	{
		path_ << root;
	}

	assetpath & assetpath::operator<<(const fschar_t * part)
	{
		path_ << os::pathsep() << part;
		return *this;
	}

	const fschar_t * assetpath::c_str() 
	{
		buff_ = path_.str();
		return buff_.c_str();
	}
}