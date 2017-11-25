#include "AlienSmall.h"

AlienSmall::AlienSmall()
{
}

AlienSmall::AlienSmall(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ALIENSMALL)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite AlienSmall");
	}

	this->sprite->setData(IndexManager::getInstance()->alienSmallR);
}


AlienSmall::~AlienSmall()
{
	delete this->sprite;
}

void AlienSmall::update(float dt)
{
}

void AlienSmall::draw()
{
	this->sprite->draw();
}
