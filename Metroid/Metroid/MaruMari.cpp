#include "MaruMari.h"

MaruMari::MaruMari()
{
}

MaruMari::MaruMari(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::MARUMARI)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite MaruMari");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->maruMari, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();
}


MaruMari::~MaruMari()
{
	delete this->anim;
	delete this->sprite;
}

void MaruMari::update(float dt)
{
	this->anim->update(dt);
}

void MaruMari::draw()
{
	this->sprite->draw();
}
