#include <moster/core.hpp>
#include <irrlicht.h>
#include <driverChoice.h>
#include <vector>

typedef irr::core::vector3df vec3df;
typedef irr::video::SColor color;

using namespace moster;
using namespace irr;

namespace moster { namespace irrlicht
{
	class wanderer
	{
	private:

		irr::scene::ISceneNode * node_;
		irr::scene::ITerrainSceneNode * terrain_;

	public:

		wanderer(irr::scene::ISceneNode * node, 
			irr::scene::ITerrainSceneNode * terrain);

		void update(const u32 time_delta_msec);
	};

	wanderer::wanderer(scene::ISceneNode * node, 
			irr::scene::ITerrainSceneNode * terrain) :
		node_(node),
		terrain_(terrain)
	{ }

	void wanderer::update(const u32 time_delta_msec)
	{
		auto ft = static_cast<float>(time_delta_msec);
		auto pos = node_->getPosition();
		auto r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		pos.X += (r - 0.5f) * ft * 0.1f;
		r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		pos.Z += (r - 0.5f) * ft * 0.1f;
		pos.Y = 2.0f + terrain_->getHeight(pos.X, pos.Z);
		node_->setPosition(pos);
	}

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

			controller();

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

		const irr::f32 max_height_;
		const irr::f32 min_height_;
		const irr::f32 zoom_speed_;

		irr::f32 angle_;
		irr::scene::ICameraSceneNode * camera_;
		controller controller_;
		irr::f32 height_;
		irr::f32 look_distance_;
		receiver receiver_;
		const irr::f32 spin_;
		irr::scene::ITerrainSceneNode * terrain_;
		const irr::f32 track_speed_;

	public:

		spincam(irr::scene::ICameraSceneNode * camera, 
			irr::scene::ITerrainSceneNode * terrain, 
			irr::f32 track_speed = 0.05f);

		receiver & key_receiver();

		void update(const unsigned int time_delta_msec);
		
	};

	spincam::spincam(scene::ICameraSceneNode * camera, 
			scene::ITerrainSceneNode * terrain, f32 track_speed) :
		angle_(0.0f),
		camera_(camera),
		controller_(),
		height_(60.0f),
		look_distance_(40.0f),
		receiver_(this->controller_),
		spin_(0.05f),
		terrain_(terrain),
		track_speed_(track_speed),
		max_height_(100.0f),
		min_height_(40.0f),
		zoom_speed_(0.05f)
	{ 
		camera_->setPosition(vec3df(0.0f, height_, 0.0f));
		update(0u);
	}

	spincam::receiver & spincam::key_receiver()
	{
		return receiver_;
	}

	void spincam::update(const unsigned int time_delta_msec)
	{
		const auto ft = static_cast<float>(time_delta_msec);
		if (controller_.is_active(controller::modifier::MD_ZOOMIN))
		{
			height_ -= zoom_speed_ * ft;
		}
		else if (controller_.is_active(controller::cmodifier::MD_ZOOMOUT))
		{
			height_ += zoom_speed_ * ft;
		}
		if (controller_.is_active(controller::modifier::MD_SPINLEFT))
		{
			angle_ += spin_ * ft;
		}
		else if (controller_.is_active(controller::cmodifier::MD_SPINRIGHT))
		{
			angle_ -= spin_ * ft;
		}
		auto targetdir = vec3df(0.0f, 0.0f, 1.0f);
		targetdir.normalize();
		targetdir.rotateXYBy(45.0f);
		targetdir.rotateXZBy(angle_);
		auto pos = camera_->getPosition();
		if (controller_.is_active(controller::modifier::MD_FORWARD))
		{ 
			pos += (track_speed_ * ft * vec3df(targetdir.X, 0.0f, targetdir.Z));
		}
		else if (controller_.is_active(controller::cmodifier::MD_BACKWARD))
		{ 
			pos	-= (track_speed_ * ft * vec3df(targetdir.X, 0.0f, targetdir.Z));
		}
		pos.Y = height_ + terrain_->getHeight(pos.X, pos.Z);
		camera_->setPosition(pos);
		camera_->setTarget(vec3df(pos.X, 0.0f, pos.Z)
			+ (look_distance_ * vec3df(targetdir.X, 0.0f, targetdir.Z)));
	}

	spincam::controller::controller() :
		cmodifier_mask_(0),
		modifier_mask_(0)
	{ }

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
	logger logger("main", logger::Info);
	logger.log(logger::Info, "hello...");
	
	if (2 > argc)
	{
		logger.log(logger::Error, "not enough arguments");
		return 1;
	}
	
#ifdef WIN32
	wchar_t assets_path_w[256];
	os::mbstowcs(assets_path_w, 256, argv[1], strlen(argv[1]));
	char assets_path[256];
	os::wcrtomb(assets_path, 256, assets_path_w, wcslen(assets_path_w));
#else
	auto assets_path = argv[1];
#endif
	logger.log(logger::Info, "getting assets from %s", assets_path);

	auto driverType = irr::driverChoiceConsole();
	if (irr::video::EDT_COUNT == driverType)
	{
		return 1;
	}

	// setup device
	irr::SIrrlichtCreationParameters params;
	params.DriverType = driverType;
	params.AntiAlias = 1;
	params.WindowSize = core::dimension2d<u32>(1024, 768);
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
	cam->setFarValue(200.0f);

	assetpath hmpath(assets_path);
	hmpath << "heightmap" << "grass1-height.png";
	auto terrain = smgr->addTerrainSceneNode(
		hmpath.c_str(),
		0,					
		-1,
		vec3df(-256.0f, 0.f, -256.0f),		
		vec3df(0.f, 0.f, 0.f),
		vec3df(1.0f, 0.5f, 1.0f),
		video::SColor(255,255,255,255),
		4,
		scene::ETPS_9,
		5
	);

	std::vector<irrlicht::wanderer *> wanderers;
	for (int i = 0; i < 1000; i++)
	{
		auto c = smgr->addSphereSceneNode(1.0);
		c->getMaterial(0).DiffuseColor.set(255, 255, 0, 0);
		c->getMaterial(0).PointCloud = true;
		auto w = new irrlicht::wanderer(c, terrain);
		w->update(0u);
		wanderers.push_back(w);
	}

	irrlicht::spincam spincam(cam, terrain);
	device->setEventReceiver(&(spincam.key_receiver()));

	smgr->setAmbientLight(video::SColorf(0.75, 0.75, 0.75));

	assetpath cmpath(assets_path);
	cmpath << "colormap" << "grass1-color.png";
	auto cmaptext = vdrv->getTexture(cmpath.c_str());
	terrain->setMaterialTexture(0, cmaptext);
	terrain->scaleTexture(1.0f, 1.0f);
	terrain->setMaterialType(irr::video::EMT_SOLID);

	const size_t wincapsz = 64;
	wchar_t wincap[wincapsz];
	auto last_time = device->getTimer()->getTime();
	while (device->run())
	{
		if (device->isWindowActive())
		{
			auto fps = vdrv->getFPS();
			auto t = device->getTimer()->getTime();
			auto tdiff = t - last_time;
			os::sprintf<wchar_t>(wincap, wincapsz, L"moster (%d fps)", fps);
			device->setWindowCaption(wincap);
			spincam.update(tdiff);
			for (unsigned int i = 0; i < wanderers.size(); i++)
			{
				wanderers[i]->update(tdiff);
			}
			vdrv->beginScene(true, true, irr::video::SColor(255, 64, 64, 64));
			smgr->drawAll();
			vdrv->endScene();
			last_time = t;
		}
	}

	device->drop();

    return 0;
}

