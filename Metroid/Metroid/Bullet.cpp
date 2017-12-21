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

	this->indexSprite = IndexManager::getInstance()->samusYellowBulletNormal;
	this->indexEffect = IndexManager::getInstance()->samusYellowBulletNormalColliding;
	// Set Data for sprite
	this->sprite->setData(indexSprite);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);

	this->distance = 0;
	this->isCollided = false;
	this->timer = 0;
	this->listCollide = new list<CollisionReturn>();

	this->dame = 1; // se setup lai sau

	this->distanceShoot = DISTANCE_SHOOT;
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
		if (i->object->isActivitied())
		{
			switch (i->object->getId())
			{
				case eID::WALL : case eID::GATERED:
					this->sprite->setData(indexEffect);
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;

					switch (i->direction)
					{
					case CollideDirection::LEFT: case CollideDirection::RIGHT:
						this->setPositionX(i->positionCollision);
						break;
					case CollideDirection::TOP:
						this->setPositionY(i->positionCollision);
						break;
					default:
						break;
					}

					break;

				case eID::GATEBLUE:
				{
					this->sprite->setData(indexEffect);
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;


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
					this->velocity = VECTOR2ZERO;

					break;
				}


				case eID::ZOMMER:
				{
					Zommer* zommer = static_cast<Zommer*>((*i).object);
					zommer->setCold(true);
					this->velocity = VECTOR2ZERO;

					break;
				}

				default:
					break;
			}
		}
	}

	this->listCollide->clear();
}


void Bullet::update(float dt)
{
	if (!this->isCollided)
	{
		if (this->distance < this->distanceShoot)
		{
			this->distance += VELOCITY_BULLET*dt;
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
	setBoundCollision();

	this->distance = 0;
	this->setStatus(eStatus::RUNNING);
	this->isActivity = true;
}

void Bullet::returnPool()
{
	this->setStatus(eStatus::ENDING);
	this->setPosition(VECTOR2ZERO);
	setBoundCollision();
	this->sprite->setData(indexSprite);
	isCollided = false;
	this->timer = 0;
	this->isActivity = false;
	this->velocity = VECTOR2ZERO;
}

list<CollisionReturn>* Bullet::getListCollide()
{
	return this->listCollide;
}

float Bullet::getDistanceShoot()
{
	return this->distanceShoot;
}

void Bullet::setDistanceShoot(float distan)
{
	this->distanceShoot = distan;
}

void Bullet::setIceBullet()
{
	this->indexSprite = IndexManager::getInstance()->samusPinkBulletIce;
	this->indexEffect = IndexManager::getInstance()->samusPinkBulletIceColliding;
	this->sprite->setData(indexSprite);
}

