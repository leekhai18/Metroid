#include "Metroid.h"
#include "Sound.h"
#include "ObjectManager.h"
#include "Collision.h"

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
	delete skree;
	delete zeb;
	delete rio;
	delete zommer;
	delete fpsText;

	ObjectManager::getInstance()->release();
	Collision::getInstance()->release();
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

	camera = new Camera(GAME_WIDTH, GAME_HEIGHT);
	camera->setPosition(VECTOR2(640, 3240));

	mapBrinstar->setCamera(camera);

	samus = new Samus(textureManager, graphics, input);
	samus->setCamera(this->camera);

	skree = new Skree(textureManager, graphics);
	zeb = new Zeb(textureManager, graphics);
	waver = new Waver(textureManager, graphics);
	zommer = new Zommer(textureManager, graphics);
	rio = new Rio(textureManager, graphics);

	fpsText = new Text("FPS: 0", eFont::body, 8, VECTOR2(GAME_WIDTH - 70, 5), GraphicsNS::WHITE, false, true);
	fpsText->initialize(graphics);
}

void Metroid::update(float dt)
{
	samus->update(dt);
	
	skree->setTarget(VECTOR2(samus->getPosition().x + samus->getSprite()->getWidth()*0.5f, GAME_HEIGHT*0.8f));
	skree->update(dt);

	if (zeb->isInStatus(eStatus::START))
		zeb->init(samus->getPosition());
	zeb->update(dt);
	
	waver->update(dt);

	zommer->update(dt);
	
	rio->setTarget(VECTOR2(samus->getPosition().x, samus->getPosition().y), samus->isInStatus(eStatus::ROLLING));
	rio->update(dt);

	this->camera->update(dt);
}

void Metroid::handleInput(float dt)
{
	samus->handleInput(dt);
}

void Metroid::collisions(float dt)
{
	samus->handleInput(dt);
	ObjectManager::getInstance()->onCheckCollision(samus, dt);
	//for (size_t i = 0; i < length; i++)
	//{
	//	ObjectManager::getInstance()->onCheckCollision(i, dt);
	//}
}

void Metroid::render()
{
	// BEGIN
	this->getGraphics()->spriteBegin();

	samus->draw();
	mapBrinstar->draw();
	skree->draw();
	zeb->draw();
	waver->draw();
	zommer->draw();
	rio->draw();

	// END
	this->getGraphics()->spriteEnd();

	fpsText->setText("FPS: " + std::to_string((int)this->fps));
	fpsText->draw();
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
