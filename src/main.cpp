#include <moster/logger.hpp>
#include <irrlicht.h>
#include <driverChoice.h>
#include <string>
#ifdef _MSC_VER
#include <tchar.h>
#endif

typedef irr::core::vector3df vec3df;
typedef irr::video::SColor color;

using namespace moster;

class keyhandler : public irr::IEventReceiver
{

private:

	bool is_in_debug_mode_;
	irr::scene::ICameraSceneNode * camera_;

public:
 
	explicit keyhandler(irr::scene::ICameraSceneNode * camera);

	bool OnEvent(const irr::SEvent & event);
};

keyhandler::keyhandler(irr::scene::ICameraSceneNode * camera) : 
	camera_(camera),
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

namespace moster { namespace os 
{

	char * makepath(char * dest, size_t max, const char * left, const char * right);

#ifdef WIN32
	char pathchar() { return '\\'; }
#else
	char pathchar() { retur '/'; }
#endif

	char * makepath(char * dest, size_t max, const char * left, const char * right)
	{
		strncpy_s(dest, max, left, strnlen_s(right, max));

		*next = pathchar();
		next++;
		return strncpy(next, right, max - (next - left));
	}

} }

int main(int argc, char ** argv)
{
	logger logger("main", logger::Info);
	logger.log(logger::Info, "hello...");

	if (1 > argc)
	{
		logger.log(logger::Error, "not enough arguments");
		return 1;
	}
	const char * assets_path = argv[0];
	logger.log(logger::Info, "getting assets from %s", assets_path);

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

	// setup video driver
	auto vdrv = device->getVideoDriver();

	// setup scene graph
	auto smgr = device->getSceneManager();
	auto cam = smgr->addCameraSceneNode();

	cam->setPosition(vec3df(0.0f, 3000.0f, 0.0f));
	cam->setTarget(vec3df(0.0f, 0.0f, 0.0f));
	cam->setFarValue(100.0f);
	keyhandler key_handler(cam);
	device->setEventReceiver(&key_handler);

	char temp_path[128];
	os::makepath(temp_path, 128, assets_path, "heightmap");
	os::makepath(temp_path, 128, temp_path, "grass1-height.png");
	auto terrain = smgr->addTerrainSceneNode(
		temp_path,
		NULL,
		-1,
		vec3df(0.0f, 0.0f, 0.0f),
		vec3df(0.0f, 0.0f, 0.0f),
		vec3df(100.0f, 5.0f, 100.0f),
		color(255, 255, 0, 0),
		2,
		irr::scene::ETPS_9,
		4
	);

	os::makepath(temp_path, 128, assets_path, "colormap");
	os::makepath(temp_path, 128, temp_path, "grass1-color.png");
	auto cmap = vdrv->getTexture(temp_path);
	terrain->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	terrain->setMaterialTexture(0, cmap);
	terrain->scaleTexture(1.0, 0.0);
	while (device->run())
	{
		auto fps = vdrv->getFPS();
		logger.log(
			moster::logger::Info, 
			"the time is %d, fps is %d",
			device->getTimer()->getTime(),
			fps
		);
		vdrv->beginScene(true, true);
		
		vdrv->endScene();
	}

	device->drop();

    return 0;
}

