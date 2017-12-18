#include "MotherBrain.h"

MotherBrain::MotherBrain()
{
}

MotherBrain::MotherBrain(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::MOTHERBRAIN)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite MotherBrain");
	}

	this->sprite->setData(IndexManager::getInstance()->bossDragonL);
}


MotherBrain::~MotherBrain()
{
	delete this->sprite;
}

void MotherBrain::update(float dt)
{
}

void MotherBrain::draw()
{
	this->sprite->draw();
}
