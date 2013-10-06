#include <moster/core.hpp>
#include <irrlicht.h>
#include <driverChoice.h>

typedef irr::core::vector3df vec3df;
typedef irr::video::SColor color;

using namespace moster;

namespace moster { namespace irrlicht
{
	class spincam
	{
	private:

		class controller
		{
		private:

			int cmodifier_mask_;
			int modifier_mask_;

		public:

			enum cmodifier
			{
				MD_CNONE =		0x0000,
				MD_BACKWARD =	0x0001,
				MD_SPINRIGHT =	0x0002,
				MD_ZOOMOUT =	0x0004
			};

			enum modifier
			{
				MD_NONE =		0x0000,
				MD_FORWARD =	0x0001,
				MD_SPINLEFT =	0x0002,
				MD_ZOOMIN =		0x0004
			};

			void clear();

			void clear(cmodifier modifier);

			void clear(modifier modifier);

			bool is_active(modifier modifier) const;

			bool is_active(cmodifier modifier) const;

			void set(cmodifier modifier);

			void set(modifier modifier);

		};

	public:

		class receiver : public irr::IEventReceiver
		{
		private:

			controller & controller_;

		public:

			receiver(controller & controller);

			bool OnEvent(const irr::SEvent & event);
		};
		
	private:

		::irr::scene::ICameraSceneNode * camera_;
		controller controller_;
		receiver receiver_;
		const vec3df slide_;
		const vec3df spin_;

	public:

		spincam(irr::scene::ICameraSceneNode * camera);

		receiver & key_receiver();

		void update(const unsigned int time_delta_msec);
		
	};

	spincam::spincam(irr::scene::ICameraSceneNode * camera) :
		camera_(camera),
		slide_(50.0f, 0.0f, 50.0f),
		spin_(96.0f, 0.0f, -96.0f),
		controller_(),
		receiver_(this->controller_)
	{ }

	spincam::receiver & spincam::key_receiver()
	{
		return receiver_;
	}

	void spincam::update(const unsigned int time_delta_msec)
	{
		if (controller_.is_active(controller::modifier::MD_FORWARD))
		{
			camera_->setPosition(camera_->getPosition() + slide_);
			camera_->setTarget(camera_->getTarget() + slide_);
		}
		else if (controller_.is_active(controller::cmodifier::MD_BACKWARD))
		{
			camera_->setPosition(camera_->getPosition() - slide_);
			camera_->setTarget(camera_->getTarget() - slide_);
		}
		if (controller_.is_active(controller::modifier::MD_SPINLEFT))
		{
			camera_->setTarget(camera_->getTarget() - spin_);
		}
		else if (controller_.is_active(controller::cmodifier::MD_SPINRIGHT))
		{
			camera_->setTarget(camera_->getTarget() + spin_);
		}
	}

	void spincam::controller::clear()
	{
		modifier_mask_ = 0;
		cmodifier_mask_ = 0;
	}

	void spincam::controller::clear(spincam::controller::cmodifier modifier)
	{
		cmodifier_mask_ &= (~modifier);
	}

	void spincam::controller::clear(spincam::controller::modifier modifier)
	{
		modifier_mask_ &= (~modifier);
	}

	bool spincam::controller::is_active(spincam::controller::cmodifier modifier) const
	{
		return 0 != (modifier & cmodifier_mask_)
			&& 0 == (modifier & modifier_mask_);
	}

	bool spincam::controller::is_active(spincam::controller::modifier modifier) const
	{
		return 0 != (modifier & modifier_mask_) 
			&& 0 == (modifier & cmodifier_mask_);
	}

	void spincam::controller::set(spincam::controller::cmodifier modifier)
	{
		cmodifier_mask_ |= modifier;
	}

	void spincam::controller::set(spincam::controller::modifier modifier)
	{
		modifier_mask_ |= modifier;
	}

	spincam::receiver::receiver(spincam::controller & controller) :
		controller_(controller)
	{ }

	bool spincam::receiver::OnEvent(const irr::SEvent & event)
	{
		if (irr::EET_KEY_INPUT_EVENT == event.EventType)
		{
			spincam::controller::modifier m = spincam::controller::modifier::MD_NONE;
			spincam::controller::cmodifier cm = spincam::controller::cmodifier::MD_CNONE;
			switch (event.KeyInput.Key)
			{
				case irr::KEY_KEY_W:
					m = spincam::controller::modifier::MD_FORWARD;
					break;
				case irr::KEY_KEY_S:
					cm = spincam::controller::cmodifier::MD_BACKWARD;
					break;
				case irr::KEY_KEY_A:
					m = spincam::controller::modifier::MD_SPINLEFT;
					break;
				case irr::KEY_KEY_D:
					cm = spincam::controller::cmodifier::MD_SPINRIGHT;
					break;
				case irr::KEY_KEY_C:
					m = spincam::controller::modifier::MD_ZOOMIN;
					break;
				case irr::KEY_KEY_V:
					cm = spincam::controller::cmodifier::MD_ZOOMOUT;
					break;
			}
			if (event.KeyInput.PressedDown)
			{
				controller_.set(m);
				controller_.set(cm);
			}
			else
			{
				controller_.clear(m);
				controller_.clear(cm);
			}
		}
		return false;
	}
} }

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

	irrlicht::spincam spincam(cam);
	device->setEventReceiver(&(spincam.key_receiver()));
	
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

