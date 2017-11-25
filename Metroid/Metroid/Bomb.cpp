#include "Bomb.h"

Bomb::Bomb()
{
}

Bomb::Bomb(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::BOMB)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bomb");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->bomb, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();
}


Bomb::~Bomb()
{
	delete this->anim;
	delete this->sprite;
}

void Bomb::update(float dt)
{
	this->anim->update(dt);
}

void Bomb::draw()
{
	this->sprite->draw();
}
