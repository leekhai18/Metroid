#include "IceBeam.h"

IceBeam::IceBeam()
{
}

IceBeam::IceBeam(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ICEBEAM)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite IceBeam");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->iceBeam, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();
}


IceBeam::~IceBeam()
{
	delete this->anim;
	delete this->sprite;
}

void IceBeam::update(float dt)
{
	this->anim->update(dt);
}

void IceBeam::draw()
{
	this->sprite->draw();
}
