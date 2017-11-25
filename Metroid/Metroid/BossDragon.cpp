#include "BossDragon.h"

BossDragon::BossDragon()
{
}

BossDragon::BossDragon(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::BOSSDRAGON)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite BossDragon");
	}

	this->sprite->setData(IndexManager::getInstance()->bossDragonL);
}


BossDragon::~BossDragon()
{
	delete this->sprite;
}

void BossDragon::update(float dt)
{
}

void BossDragon::draw()
{
	this->sprite->draw();
}
