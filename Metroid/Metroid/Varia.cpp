#include "Varia.h"

Varia::Varia()
{
}

Varia::Varia(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::VARIA)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Varia");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->varia, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();
}


Varia::~Varia()
{
	delete this->anim;
	delete this->sprite;
}

void Varia::update(float dt)
{
	this->anim->update(dt);
}

void Varia::draw()
{
	this->sprite->draw();
}
