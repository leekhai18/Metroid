#include "EnergyTank.h"

EnergyTank::EnergyTank()
{
}

EnergyTank::EnergyTank(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ENERGYTANK)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite EnergyTank");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->energyTank, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();
}


EnergyTank::~EnergyTank()
{
	delete this->anim;
	delete this->sprite;
}

void EnergyTank::update(float dt)
{
	this->anim->update(dt);
}

void EnergyTank::draw()
{
	this->sprite->draw();
}
