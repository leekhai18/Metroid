#include "LongBeam.h"

LongBeam::LongBeam()
{
}

LongBeam::LongBeam(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::LONGBEAM)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite LongBeam");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->longBeam, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();
}


LongBeam::~LongBeam()
{
	delete this->anim;
	delete this->sprite;
}

void LongBeam::update(float dt)
{
	this->anim->update(dt);
}

void LongBeam::draw()
{
	this->sprite->draw();
}
