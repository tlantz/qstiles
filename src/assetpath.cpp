#include <moster/assetpath.hpp>

namespace moster
{
	assetpath::assetpath(const os::string & root)
	{
		if (root.empty())
		{
			path_ << os::pathsep();
		}
		else
		{
			path_ << root;
		}
	}

	assetpath & assetpath::operator<<(const os::string & part)
	{
		path_ << os::pathsep() << part;
		return *this;
	}

	os::string assetpath::str() const
	{
		return path_.str();
	}

}