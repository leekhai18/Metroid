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
	delete opsText;

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
	camera->setPosition(VECTOR2(CAM_POS_X, CAM_POS_Y));


	ObjectManager::getInstance()->init(textureManager, graphics);
	if (!ObjectManager::getInstance()->load_list(OBJECT_LAYER_BRINSTAR_JSON))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not load object layer brinstar json");
	}

	samus = new Samus(textureManager, graphics, input);

	skree = new Skree(textureManager, graphics, EnemyColors::Yellow);
	skree->initPositions(VECTOR2(840, 3212));
	zeb = new Zeb(textureManager, graphics, EnemyColors::Yellow);
	zeb->setPosition(VECTOR2(840, 3333));
	waver = new Waver(textureManager, graphics, EnemyColors::Brown);
	waver->setPosition(VECTOR2(640, 3180));
	zommer = new Zommer(textureManager, graphics, EnemyColors::Yellow);
	zommer->setPosition(VECTOR2(680, 3180));
	rio = new Rio(textureManager, graphics, EnemyColors::Yellow);
	rio->initPositions(VECTOR2(780, 3130));

	fpsText = new Text("FPS: 0", eFont::body, 8, VECTOR2(GAME_WIDTH - 70, 5), GraphicsNS::WHITE, false, true);
	fpsText->initialize(graphics);

	opsText = new Text("OPS: 0", eFont::body, 8, VECTOR2(GAME_WIDTH - 70, 15), GraphicsNS::WHITE, false, true);
	opsText->initialize(graphics);
}

void Metroid::update(float dt)
{
	samus->update(dt);
	
	skree->setTarget(VECTOR2(samus->getPosition().x + samus->getSprite()->getWidth()*0.5f, samus->getPosition().y));
	skree->update(dt);

	if (zeb->isInStatus(eStatus::START))
		zeb->init(samus->getPosition());
	zeb->update(dt);
	
	waver->update(dt);

	zommer->update(dt);
	
	rio->setTarget(VECTOR2(samus->getPosition().x, samus->getPosition().y), samus->isInStatus(eStatus::ROLLING));
	rio->update(dt);

	ObjectManager::getInstance()->update(dt);

	this->camera->update(dt);


	// Need update() some object created in object_List
	//ObjectManager::getInstance()->getObjectList()->back()->update(dt);
}

void Metroid::handleInput(float dt)
{
	samus->handleInput(dt);
}

void Metroid::collisions(float dt)
{
	ObjectManager::getInstance()->onCheckCollision(samus, dt);
}

void Metroid::render()
{
	// BEGIN
	this->getGraphics()->spriteBegin();

	samus->draw();
	mapBrinstar->draw();

	ObjectManager::getInstance()->draw();

	skree->draw();
	zeb->draw();
	waver->draw();
	zommer->draw();
	rio->draw();

	// Need draw() some object created in object_List
	//ObjectManager::getInstance()->getObjectList()->back()->draw();

	// END
	this->getGraphics()->spriteEnd();

	fpsText->setText("FPS: " + std::to_string((int)this->fps));
	fpsText->draw();

	opsText->setText("OPS: " + std::to_string(ObjectManager::getInstance()->getTotalObjectsPerFrame()));
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
