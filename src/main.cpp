#include <moster/logger.hpp>
#include <irrlicht.h>
#include <driverChoice.h>
#ifdef _MSC_VER
#include <tchar.h>
#endif

typedef irr::core::vector3df vec3df;

class keyhandler : public irr::IEventReceiver
{

private:

	bool is_in_debug_mode_;

public:

	explicit keyhandler();

	bool OnEvent(const irr::SEvent & event);
};

keyhandler::keyhandler() : 
	is_in_debug_mode_(false)
{ }

bool keyhandler::OnEvent(const irr::SEvent & event)
{
	if (irr::EET_KEY_INPUT_EVENT == event.EventType)
	{
		// TODO
	}
	return false;
}

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

	// setup device
	irr::SIrrlichtCreationParameters params;
	params.DriverType = driverType;
	params.WindowSize = irr::core::dimension2d<irr::u32>(800, 600);
	params.Fullscreen = false;
	auto device = irr::createDeviceEx(params);
	if (0 == device)
	{
		logger.log(moster::logger::Error, "unable to create device for driver %d", driverType);
		return 1; 
	}
	device->setWindowCaption(L"moster");
	keyhandler key_handler;
	device->setEventReceiver(&key_handler);

	// setup video driver
	auto vdrv = device->getVideoDriver();

	// setup scene graph
	auto smgr = device->getSceneManager();
	auto cam = smgr->addCameraSceneNode();

	cam->setPosition(vec3df(0.0f, 3000.0f, 0.0f));
	cam->setTarget(vec3df(0.0f, 0.0f, 0.0f));
	cam->setFarValue(100.0f);

	auto terrain = smgr->addTerrainSceneNode(
		"../../assets/heightmap/grass1-height.png",
		NULL,
		-1,
		vec3df(0.0f, 0.0f, 0.0f),
		vec3df(0.0f, 0.0f, 0.0f),
		vec3df(100.0f, 5.0f, 100.0f),
		irr::video::SColor(255, 255, 0, 0),
		2,
		irr::scene::ETPS_9,
		4
	);

	auto cmap = vdrv->getTexture("../../assets/colormap/grass1-color.png");
	terrain->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	terrain->setMaterialTexture(0, cmap);
	terrain->scaleTexture(1.0, 0.0);

	while (device->run())
	{
		logger.log(moster::logger::Info, "the time is %d",
			device->getTimer()->getTime());
		vdrv->beginScene(true, false);
		vdrv->endScene();
	}

	device->drop();

    return 0;
}

