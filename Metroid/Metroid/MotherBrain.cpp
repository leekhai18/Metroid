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

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->motherBrain, NUM_FRAMES_MOTHER, 0.1f);
	this->anim->start();
}


MotherBrain::~MotherBrain()
{
	delete this->sprite;
	delete anim;
}

void MotherBrain::update(float dt)
{
	this->anim->update(dt);
}

void MotherBrain::draw()
{
	this->sprite->draw();
}
