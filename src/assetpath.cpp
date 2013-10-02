#include <moster/assetpath.hpp>

namespace moster
{
	assetpath::assetpath(const std::string & root)
	{
		if (root.empty())
		{
#ifdef WIN32
			path_ << ".\\";
#else
			path_ << "./";
#endif
		}
		else
		{
			path_ << root;
		}
	}

	assetpath & assetpath::operator<<(const std::string & part)
	{
#ifdef WIN32
		path_ << '\\' << part;
#else
		path_ << '/' << part;
#endif
		return *this;
	}

	std::string assetpath::str() const
	{
		return path_.str();
	}

}