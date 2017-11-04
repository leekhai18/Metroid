#include "Metroid.h"


Metroid::Metroid()
{
	this->spriteManger = SpriteManager::getInstance();
	this->textureManager = new TextureManager();
}

Metroid::~Metroid()
{
	delete samus;
	textureManager->onLostDevice();
	spriteManger->releaseAll();

	delete skree;
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
}

void Metroid::update(float dt)
{
	samus->update(dt);
	
	skree->setTarget(VECTOR2(samus->getPosition().x + samus->getSprite()->getWidth()*0.5f, samus->getPosition().y));
	skree->update(dt);
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
}

void Metroid::releaseAll()
{
	GameManager::releaseAll();
}

void Metroid::resetAll()
{
	GameManager::resetAll();
}
