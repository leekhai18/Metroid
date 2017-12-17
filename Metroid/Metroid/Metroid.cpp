#include "Metroid.h"
#include "Sound.h"
#include "ObjectManager.h"
#include "Collision.h"
#include "GameDebug.h"

Metroid::Metroid()
{
	this->spriteManger = SpriteManager::getInstance();
	this->textureManager = new TextureManager();

	// new map component
	this->tileset = new TextureManager();
	this->mapInfo = new MapInfo();
	this->mapBrinstar = new Map();

	instance = this;
}

Metroid::~Metroid()
{
	textureManager->onLostDevice();
	spriteManger->releaseAll();

	tileset->onLostDevice();
	delete mapInfo;
	delete mapBrinstar;

	delete samus;
	delete fpsText;
	delete opsText;

	ObjectManager::getInstance()->release();
	Collision::getInstance()->release();
	//GameDebug::getInstance()->release();
}

Metroid* Metroid::instance = nullptr;
Metroid * Metroid::getInstance()
{
	if (instance == nullptr)
		instance = new Metroid();

	return instance;
}

void Metroid::initialize(HWND hwnd) 
{
	GameManager::initialize(hwnd); // graphics was init inside

	if (!spriteManger->initialize(SOURCE_JSON))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not  load json");
	}

	if (!textureManager->initialize(graphics, SOURCE_IMAGE))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not load spite sheet");
	}

	if (!tileset->initialize(graphics, TILESET_IMAGE))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not load tileset");
	}

	if (!mapInfo->initialize(MAP_BRINSTAR_JSON))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not load map info");
	}

	if (!mapBrinstar->initialize(graphics, tileset, mapInfo))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not initalize map brinstar");
	}


	camera = new Camera(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	camera->setPosition(VECTOR2(CAM_POS_X, CAM_POS_Y));

	samus = new Samus(textureManager, graphics, input);

	ObjectManager::getInstance()->init(textureManager, graphics, samus);
	if (!ObjectManager::getInstance()->load_list(OBJECT_LAYER_BRINSTAR_JSON))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not load object layer brinstar json");
	}
	if (!ObjectManager::getInstance()->load_quatree(QUADTREE))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not load object layer brinstar json");
	}
	GameDebug::getInstance()->initialize(graphics);
	GameDebug::getInstance()->setCount(5);

	fpsText = new Text("FPS: 0", eFont::body, 8, VECTOR2(VIEWPORT_WIDTH - 70, 5), GraphicsNS::WHITE, false, true);
	fpsText->initialize(graphics);

	opsText = new Text("OPS: 0", eFont::body, 8, VECTOR2(VIEWPORT_WIDTH - 70, 15), GraphicsNS::WHITE, false, true);
	opsText->initialize(graphics);
}


void Metroid::handleInput(float dt)
{
	samus->handleInput(dt);
}

void Metroid::collisions(float dt)
{
	ObjectManager::getInstance()->onCheckCollision(dt);
}

void Metroid::update(float dt)
{
	ObjectManager::getInstance()->update(dt);
	samus->update(dt);
	this->camera->update(dt);
}

void Metroid::render()
{
	ObjectManager::getInstance()->setTotalObjectPerFrame(0);

	// BEGIN
	this->getGraphics()->spriteBegin();

	samus->draw();
	ObjectManager::getInstance()->draw();
	mapBrinstar->draw();


	// END
	this->getGraphics()->spriteEnd();


	// bound line
	//GameDebug::getInstance()->draw(); // ko chỉ transform là đc.. cần flip nữa

	fpsText->setText("FPS: " + std::to_string((int)this->fps));
	fpsText->draw();

	opsText->setText("DPS: " + std::to_string(ObjectManager::getInstance()->getTotalObjectsPerFrame()));
	opsText->draw();
}

void Metroid::releaseAll()
{
	GameManager::releaseAll();
}

void Metroid::resetAll()
{
	GameManager::resetAll();
}


HWND Metroid::getCurrentHWND()
{
	return this->hwnd;
}
