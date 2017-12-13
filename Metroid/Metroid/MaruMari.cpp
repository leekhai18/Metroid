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

	this->explosion = new Animation(this->sprite, IndexManager::getInstance()->samusYellowExplosion, 2, 0.1f, false);
}


MaruMari::~MaruMari()
{
	delete this->anim;
	delete this->sprite;
	delete this->explosion;
}

void MaruMari::update(float dt)
{
	if (sprite != nullptr)
	{
		this->anim->update(dt);
		this->explosion->update(dt);
		if (this->explosion->isFinished())
		{
			delete sprite;
			sprite = nullptr;
		}
	}
}

void MaruMari::draw()
{
	if (sprite != nullptr)
		this->sprite->draw();
}

void MaruMari::startExplosion()
{
	this->anim->stop();
	this->explosion->start();
	this->setOrigin(VECTOR2(0.5f, 0.5f));
	this->setPosition(getPosition().x + getSprite()->getWidth()*0.5f, getPosition().y - getSprite()->getHeight()*0.5f);
}
