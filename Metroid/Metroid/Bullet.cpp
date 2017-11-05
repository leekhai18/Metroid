#include "Bullet.h"



Bullet::Bullet(TextureManager * textureM, Graphics * graphics) : BaseObject()
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bullet");
	}

	// Set Data for sprite
	const SpriteData *data = &(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowBulletNormal]);
	this->sprite->setSpriteDataRect(data->rect);
	this->sprite->setSpriteWidth(data->width);
	this->sprite->setSpriteHeigth(data->height);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->startPosition = VECTOR2ZERO;
	this->setPosition(VECTOR2ZERO);

	t = 0;
}

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::update(float dt)
{
	if (t < 1)
	{
		t += dt;
		this->setPosition((1 - t)*this->startPosition + t*this->target);
	}
	else
	{
		this->setStatus(eStatus::ENDING);
	}
}

void Bullet::draw()
{
	this->sprite->draw();
}

void Bullet::init(VECTOR2 stPosition, VECTOR2 target)
{
	this->startPosition = stPosition;
	this->target = target;

	this->setStatus(eStatus::RUNNING);
	t = 0;
}
