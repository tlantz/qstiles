#include <moster/logger.hpp>
#include <irrlicht.h>

#ifdef _MSC_VER
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char ** argv)
#endif
{
    moster::logger logger("main", moster::logger::Info);
	logger.log(moster::logger::Info, "hello...");
    return 0;
}

