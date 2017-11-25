#include "AlienBig.h"

AlienBig::AlienBig()
{
}

AlienBig::AlienBig(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ALIENBIG)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite AlienBig");
	}

	this->sprite->setData(IndexManager::getInstance()->alienBigR);
}


AlienBig::~AlienBig()
{
	delete this->sprite;
}

void AlienBig::update(float dt)
{
}

void AlienBig::draw()
{
	this->sprite->draw();
}
