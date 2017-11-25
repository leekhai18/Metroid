#include "MissileRocket.h"

MissileRocket::MissileRocket()
{
}

MissileRocket::MissileRocket(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::MISSILEROCKET)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite MissileRocket");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->missileRocket, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();
}


MissileRocket::~MissileRocket()
{
	delete this->anim;
	delete this->sprite;
}

void MissileRocket::update(float dt)
{
	this->anim->update(dt);
}

void MissileRocket::draw()
{
	this->sprite->draw();
}
