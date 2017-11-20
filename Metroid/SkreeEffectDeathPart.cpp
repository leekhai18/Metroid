#include "SkreeEffectDeathPart.h"

#define RATE_BEZIER 4


SkreeEffectDeathPart::SkreeEffectDeathPart(TextureManager * textureM, Graphics * graphics) : BaseObject()
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite SkreeEffectDeath");
	}

	// Set Data for sprite
	this->sprite->setData(IndexManager::getInstance()->skreeDeathEffect);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	t = 0;
}

SkreeEffectDeathPart::SkreeEffectDeathPart()
{
}

SkreeEffectDeathPart::~SkreeEffectDeathPart()
{
}

void SkreeEffectDeathPart::init(VECTOR2 target, VECTOR2 startPosition)
{
	this->target = target;
	this->startPosition = startPosition;
	this->setPosition(startPosition);
}

void SkreeEffectDeathPart::update(float dt)
{
	if (!this->isInStatus(eStatus::JUMPING))
	{
		if (t < 1)
		{
			t += dt * RATE_BEZIER;
			this->setPosition((1 - t)*this->startPosition + t*this->target);
		}
		else
			this->setStatus(eStatus::JUMPING);
	}
}

void SkreeEffectDeathPart::draw()
{
	this->sprite->draw();
}

void SkreeEffectDeathPart::release()
{
	delete this->sprite;
	this->sprite = nullptr;
}

