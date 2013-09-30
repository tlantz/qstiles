#include <moster/logger.hpp>
#include <irrlicht.h>
#include <driverChoice.h>
#ifdef _MSC_VER
#include <tchar.h>
#endif

#ifdef _MSC_VER
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char ** argv)
#endif
{
    moster::logger logger("main", moster::logger::Info);
	logger.log(moster::logger::Info, "hello...");
	auto driverType = irr::driverChoiceConsole();
	if (irr::video::EDT_COUNT == driverType)
	{
		return 1;
	}

	irr::SIrrlichtCreationParameters params;
	params.DriverType = driverType;
	params.WindowSize = irr::core::dimension2d<irr::u32>(800, 600);
	params.Fullscreen = true;
	params.Vsync = true;
	auto device = irr::createDeviceEx(params);
	if (0 == device)
	{
		logger.log(moster::logger::Error, "unable to create device for driver %d", driverType);
		return 1; 
	}



    return 0;
}

