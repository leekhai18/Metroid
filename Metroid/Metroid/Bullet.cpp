#include "Bullet.h"
#include "BulletPool.h"
#include "Collision.h"
#include "GateBlue.h"
#include "MaruMari.h"
#include "GameLog.h"
#include "Skree.h"
#include "Zommer.h"
#define WIDTH_BULLET_HALF 1
#define HEIGHT_BULLET_HALF 1

Bullet::Bullet(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::BULLET)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bullet");
	}

	// Set Data for sprite
	this->sprite->setData(IndexManager::getInstance()->samusYellowBulletNormal);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);

	this->distance = 0;
	this->isCollided = false;
	this->timer = 0;
	this->listCollide = new list<CollisionReturn>();

	this->dame = 1; // se setup lai sau
}

Bullet::Bullet()
{

}


Bullet::~Bullet()
{
	delete this->sprite;
	this->listCollide->clear();
	delete this->listCollide;
}

void Bullet::onCollision()
{
	for (auto i = this->listCollide->begin(); i != this->listCollide->end(); i++)
	{
		switch (i->object->getId())
		{
		case eID::WALL:
			this->sprite->setData(IndexManager::getInstance()->samusYellowBulletNormalColliding);
			this->isCollided = true;
			
			switch (i->direction)
			{
			case CollideDirection::LEFT : case CollideDirection::RIGHT:
				this->setPositionX(i->positionCollision);
				break;
			case CollideDirection::TOP:
				this->setPositionY(i->positionCollision);
				break;
			default:
				break;
			}

			break;

		case eID::GATEBLUE: case eID::GATERED:
		{
			this->sprite->setData(IndexManager::getInstance()->samusYellowBulletNormalColliding);
			this->isCollided = true;

			GateBlue* gate = static_cast<GateBlue*>(i->object);
			gate->setHit(true);

			switch (i->direction)
			{
			case CollideDirection::LEFT: case CollideDirection::RIGHT:
				this->setPositionX(i->positionCollision);
				break;
			default:
				break;
			}
			break;
		}

		case eID::SKREE:
		{
			Skree* skr = static_cast<Skree*>(i->object);
			skr->setBeHit(true);
			skr->decreaseHealth(this->dame);
			break;
		}


		case eID::ZOMMER:
			Zommer* zommer = static_cast<Zommer*>((*i).object);
			zommer->setCold(true);
			break;
		}
	}

	this->listCollide->clear();
}


void Bullet::update(float dt)
{
	if (!this->isCollided)
	{
		if (this->distance < DISTANCE_SHOOT)
		{
			this->distance += VELOCITY*dt;
			this->setPosition(this->getPosition().x + this->getVelocity().x*dt, this->getPosition().y + this->getVelocity().y*dt);
			setBoundCollision();
		}
		else
		{
			BulletPool::getInstance()->returnPool(this);
		}
	}
	else
	{
		timer += dt;
		if (timer > 0.1)
		{
			BulletPool::getInstance()->returnPool(this);
		}
	}
}

void Bullet::draw()
{
	this->sprite->draw();
}

void Bullet::setBoundCollision()
{
	boundCollision.left = getPosition().x - WIDTH_BULLET_HALF;
	boundCollision.right = getPosition().x + WIDTH_BULLET_HALF;
	boundCollision.top = getPosition().y + HEIGHT_BULLET_HALF;
	boundCollision.bottom = getPosition().y - HEIGHT_BULLET_HALF;
}

void Bullet::init(VECTOR2 stPosition)
{
	this->setPosition(stPosition);

	this->distance = 0;
	this->setStatus(eStatus::RUNNING);
}

void Bullet::returnPool()
{
	this->setStatus(eStatus::ENDING);
	this->setPosition(VECTOR2ZERO);
	this->sprite->setData(IndexManager::getInstance()->samusYellowBulletNormal);
	isCollided = false;
	this->timer = 0;
}

list<CollisionReturn>* Bullet::getListCollide()
{
	return this->listCollide;
}

