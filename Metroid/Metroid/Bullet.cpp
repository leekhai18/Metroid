#include "Bullet.h"
#include "BulletPool.h"
#include "Collision.h"

#define WIDTH_BULLET_HALF 3
#define HEIGHT_BULLET_HALF 3

Bullet::Bullet(TextureManager * textureM, Graphics * graphics) : BaseObject()
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bullet");
	}

	// Set Data for sprite
	this->sprite->setData(IndexManager::getInstance()->samusYellowBulletNormal);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->startPosition = VECTOR2ZERO;
	this->setPosition(VECTOR2ZERO);

	t = 0;

	this->setStatus(eStatus::START);

}

Bullet::Bullet()
{

}


Bullet::~Bullet()
{
	delete this->sprite;
}


void Bullet::update(float dt)
{
	if (t < 1)
	{
		t += dt * (1.0f / TIME_TO_TARGET);
		this->setPosition((1 - t)*this->startPosition + t*this->target);
		setBoundCollision();
	}
	else
	{
		this->setStatus(eStatus::ENDING);

		BulletPool::getInstance()->returnPool(this);
	}


}

void Bullet::draw()
{
	this->sprite->draw();
}

void Bullet::onCollision(BaseObject* object, CollisionReturn result)
{
	switch (result.idObject)
	{
	case eID::WALL:
		BulletPool::getInstance()->returnPool(this);
		break;
	default:
		break;
	}
}

void Bullet::setBoundCollision()
{
	MetroidRect rect;

	rect.left = getPosition().x - WIDTH_BULLET_HALF;
	rect.right = getPosition().x + WIDTH_BULLET_HALF;
	rect.top = getPosition().y - HEIGHT_BULLET_HALF;
	rect.bottom = getPosition().y + HEIGHT_BULLET_HALF;

	boundCollision = rect;
}

void Bullet::init(VECTOR2 stPosition, VECTOR2 target)
{
	this->startPosition = stPosition;
	this->target = target;

	t = 0;
	this->setStatus(eStatus::RUNNING);
}

void Bullet::returnPool()
{
	this->startPosition = VECTOR2ZERO;
	this->target = VECTOR2ZERO;
}
