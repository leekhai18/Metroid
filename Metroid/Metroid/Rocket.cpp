#include "Rocket.h"
#include "RocketPool.h"
#include "Collision.h"
#include "GateBlue.h"
#include "MaruMari.h"
#include "GameLog.h"
#include "Skree.h"
#include "Zommer.h"
#include "GateRed.h"
#define WIDTH_ROCKET_HALF 4
#define HEIGHT_ROCKET_HALF 4

Rocket::Rocket(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ROCKET)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite ROCKET");
	}

	// Set Data for sprite
	this->sprite->setData(IndexManager::getInstance()->rocketPinkR);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);

	this->distance = 0;
	this->isCollided = false;
	this->timer = 0;
	this->listCollide = new list<CollisionReturn>();

	this->dame = 1; // se setup lai sau
}

Rocket::Rocket()
{

}


Rocket::~Rocket()
{
	delete this->sprite;
	this->listCollide->clear();
	delete this->listCollide;
}

void Rocket::onCollision()
{
	for (auto i = this->listCollide->begin(); i != this->listCollide->end(); i++)
	{
		if (i->object->isActivitied())
		{
			switch (i->object->getId())
			{
				case eID::WALL:
					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					break;

				case eID::GATEBLUE:
				{
					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					this->isCollided = true;

					GateBlue* gateB = static_cast<GateBlue*>(i->object);
					gateB->setHit(true);
					this->velocity = VECTOR2ZERO;

					break;
				}

				case eID::GATERED:
				{
					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					this->isCollided = true;

					GateRed* gateR = static_cast<GateRed*>(i->object);
					gateR->setDurability(gateR->getDurability() - 1);
					this->velocity = VECTOR2ZERO;

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


void Rocket::update(float dt)
{
	if (!this->isCollided)
	{
		if (this->distance < DISTANCE_SHOOT_ROCKET)
		{
			this->distance += VELOCITY_ROCKET*dt;
			this->setPosition(this->getPosition().x + this->getVelocity().x*dt, this->getPosition().y + this->getVelocity().y*dt);
			setBoundCollision();
		}
		else
		{
			RocketPool::getInstance()->returnPool(this);
		}
	}
	else
	{
		timer += dt;
		if (timer > 0.1)
		{
			RocketPool::getInstance()->returnPool(this);
		}
	}
}

void Rocket::draw()
{
	this->sprite->draw();
}

void Rocket::setBoundCollision()
{
	boundCollision.left = getPosition().x - WIDTH_ROCKET_HALF;
	boundCollision.right = getPosition().x + WIDTH_ROCKET_HALF;
	boundCollision.top = getPosition().y + HEIGHT_ROCKET_HALF;
	boundCollision.bottom = getPosition().y - HEIGHT_ROCKET_HALF;
}

void Rocket::init(VECTOR2 stPosition)
{
	this->setPosition(stPosition);
	setBoundCollision();

	this->distance = 0;
	this->setStatus(eStatus::RUNNING);
	this->isActivity = true;
}

void Rocket::returnPool()
{
	this->setStatus(eStatus::ENDING);
	this->setPosition(VECTOR2ZERO);
	setBoundCollision();
	this->sprite->setData(IndexManager::getInstance()->rocketPinkR);
	isCollided = false;
	this->timer = 0;
	this->isActivity = false;
	this->velocity = VECTOR2ZERO;
}

list<CollisionReturn>* Rocket::getListCollide()
{
	return this->listCollide;
}


