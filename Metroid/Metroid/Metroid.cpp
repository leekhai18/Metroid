#include "Metroid.h"


Metroid::Metroid()
{
	this->spriteManger = SpriteManager::getInstance();
	this->textureManager = new TextureManager();
}

Metroid::~Metroid()
{
	textureManager->onLostDevice();
	spriteManger->releaseAll();

	delete samus;
	delete skree;
	delete zeb;
}

void Metroid::initialize(HWND hwnd) 
{
	GameManager::initialize(hwnd);
	if (!spriteManger->initialize(SOURCE_JSON))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not  load json");
	}
	if (!textureManager->initialize(graphics, SOURCE_IMAGE, 0, 0))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not load image");
	}

	samus = new Samus(textureManager, graphics, input);
	skree = new Skree(textureManager, graphics);
	zeb = new Zeb(textureManager, graphics);
	waver = new Waver(textureManager, graphics);
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
}

void Metroid::ai()
{

}

void Metroid::collisions()
{

}

void Metroid::render()
{
	samus->draw();
	skree->draw();
	zeb->draw();
	waver->draw();
}

void Metroid::releaseAll()
{
	GameManager::releaseAll();
}

void Metroid::resetAll()
{
	GameManager::resetAll();
}
