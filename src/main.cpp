#include <moster/core.hpp>
#include <irrlicht.h>
#include <driverChoice.h>

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
		auto oldpos = camera_->getPosition();
		auto oldlookat = camera_->getTarget();
		switch (event.KeyInput.Key)
		{
			case irr::KEY_KEY_W:
				camera_->setPosition(oldpos + vec3df(0.0, 0.0, 50.0));
				camera_->setTarget(oldlookat + vec3df(0.0, 0.0, 50.0));
				break;
			case irr::KEY_KEY_S:
				camera_->setPosition(oldpos + vec3df(0.0, 0.0, -50.0));
				camera_->setTarget(oldlookat + vec3df(0.0, 0.0, -50.0));
				break;
		}
	}
	return false; 
}

int main(int argc, char ** argv)
{
	logger logger(L"main", logger::Info);
	logger.log(logger::Info, L"hello...");
	
	if (2 > argc)
	{
		logger.log(logger::Error, L"not enough arguments");
		return 1;
	}
	os::oschar assets_path[128];
	os::mbstowcs(assets_path, 128, argv[1], strlen(argv[1]));
	logger.log(logger::Info, L"getting assets from %s", assets_path);

	auto driverType = irr::driverChoiceConsole();
	if (irr::video::EDT_COUNT == driverType)
	{
		return 1;
	}

	// setup device
	irr::SIrrlichtCreationParameters params;
	params.DriverType = driverType;
	params.AntiAlias = 3;
	auto device = irr::createDeviceEx(params);
	if (0 == device)
	{
		logger.log(moster::logger::Error, L"unable to create device for driver %d", driverType);
		return 1; 
	}
	device->setWindowCaption(L"moster");

	// setup video driver
	auto vdrv = device->getVideoDriver();

	// setup scene graph
	auto smgr = device->getSceneManager();
	auto cam = smgr->addCameraSceneNode(
		0, 
		vec3df(0,30,-40), 
		vec3df(0,5,0)
	);
	cam->setPosition(vec3df(-2000.0,10000.0,-2000.0));
	cam->setTarget(vec3df(10000.0, 0.0, 10000.0));
	cam->setFarValue(60000.0f);

	keyhandler key_handler(cam);
	device->setEventReceiver(&key_handler);
	
	auto hmpath = (assetpath(assets_path) << L"heightmap"
		<< L"grass1-height.png").str();
	auto terrain = smgr->addTerrainSceneNode(
		hmpath.c_str(),
		0,					
		-1,					
		vec3df(0.f, 0.f, 0.f),		
		vec3df(0.f, 0.f, 0.f),
		vec3df(100.f, 25.0f, 100.f),	
		irr::video::SColor(255, 255, 255, 255),
		5,
		irr::scene::ETPS_17,
		4	
	);

	auto cmpath = (assetpath(assets_path) << L"colormap"
		<< L"grass1-color.png").str();
	auto cmaptext = vdrv->getTexture(cmpath.c_str());
	terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, cmaptext);
	terrain->setMaterialTexture(1, cmaptext);
	terrain->scaleTexture(1.0f, 1.0f);
	terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
	
	const size_t wincapsz = 64;
	os::oschar wincap[wincapsz];
	while (device->run())
	{
		if (device->isWindowActive())
		{
			auto fps = vdrv->getFPS();
			os::wsprintf(wincap, wincapsz, L"moster (%d fps)", fps);
			device->setWindowCaption(L"moster");
			vdrv->beginScene(true, true, irr::video::SColor(255, 64, 64, 64));
			smgr->drawAll();
			vdrv->endScene();
		}
	}

	device->drop();

    return 0;
}

