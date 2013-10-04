#include <moster/assetpath.hpp>

namespace moster
{
	assetpath::assetpath(const os::string & root)
	{
		if (root.empty())
		{
#ifdef WIN32
			path_ << L".\\";
#else
			path_ << L"./";
#endif
		}
		else
		{
			path_ << root;
		}
	}

	assetpath & assetpath::operator<<(const os::string & part)
	{
#ifdef WIN32
		path_ << '\\' << part;
#else
		path_ << '/' << part;
#endif
		return *this;
	}

	os::string assetpath::str() const
	{
		return path_.str();
	}

}