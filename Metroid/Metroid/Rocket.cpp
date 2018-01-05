#include "Rocket.h"
#include "RocketPool.h"
#include "Collision.h"
#include "GateBlue.h"
#include "MaruMari.h"
#include "GameLog.h"
#include "GateRed.h"
#include "Skree.h"
#include "Zommer.h"
#include "Waver.h"
#include "Zeb.h"
#include "Rio.h"
#include "BossKraid.h"
#include "MotherBrain.h"
#include "AlienBig.h"
#include "AlienSmall.h"
#include "Ripper.h"
#include "Sound.h"
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

	this->dame = 3; // se setup lai sau
}

Rocket::Rocket()
{

}


Rocket::~Rocket()
{
	delete this->sprite;
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
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);

					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					break;

				case eID::GATEBLUE:
				{
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);

					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					this->isCollided = true;

					GateBlue* gateB = static_cast<GateBlue*>(i->object);
					gateB->setHit(true);
					this->velocity = VECTOR2ZERO;

					break;
				}

				case eID::GATERED:
				{
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);

					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					this->isCollided = true;

					GateRed* gateR = static_cast<GateRed*>(i->object);
					gateR->setDurability(gateR->getDurability() - 1);
					this->velocity = VECTOR2ZERO;

					break;
				}



				case eID::SKREE:
				{
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);


					Skree* skree = static_cast<Skree*>((*i).object);
					if (!skree->getHandle())
					{
						break;
					}
					this->isCollided = true;

					this->velocity = VECTOR2ZERO;

					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					if (!skree->getCold())
					{
						skree->setBeHit(true);
						skree->decreaseHealth(this->dame);

					}
					if (BulletPool::getInstance()->getCurrentIceBullet())
					{
						if (skree->getCold())
						{
							skree->setCold(false);

						}
						else
						{
							skree->setCold(true);
						}
					}

					break;
				}
				case eID::RIO:
				{
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);

					Rio* rio = static_cast<Rio*>((*i).object);
					if (!rio->getHandle())
					{
						break;
					}
					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
				
					if (!rio->getCold())
					{
						rio->setBeHit(true);
						rio->decreaseHealth(this->dame);

					}
					if (rio->getCold())
					{
						rio->setCold(false);

					}

					break;
				}
				case eID::RIPPER:
				{
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);
					
					Ripper* ripper = static_cast<Ripper*>((*i).object);

					if (ripper->getCold())
					{
						ripper->setCold(false);

					}
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					break;
				}
				case eID::ZOMMER:
				{
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);

			
					Zommer* zommer = static_cast<Zommer*>((*i).object);
					if (!zommer->getHandle())
					{
						break;
					}
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					if (!zommer->getCold())
					{
						zommer->setBeHit(true);
						zommer->decreaseHealth(this->dame);
						
					}
					if (zommer->getCold())
					{
						zommer->setCold(false);

					}

					break;
				}
				case eID::WAVER:
				{
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);

				
					Waver* waver = static_cast<Waver*>(i->object);
					if (!waver->getHandle())
					{
						break;
					}
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					if (!waver->getCold())
					{
						waver->setBeHit(true);
						waver->decreaseHealth(this->dame);
						
					}
					if (waver->getCold())
					{
						waver->setCold(false);

					}

					break;
				}
				case eID::ZEB:
				{
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);

					
					Zeb* zeb = static_cast<Zeb*>(i->object);
					if (!zeb->getHandle())
					{
						break;
					}
					this->isCollided = true;
					this->velocity = VECTOR2ZERO;
					this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
					if (!zeb->getCold())
					{
						zeb->setBeHit(true);
						zeb->decreaseHealth(this->dame);

					}
					if (zeb->getCold())
					{
						zeb->setCold(false);

					}

					break;
				}
				case eID::BOSSKRAID:
				{
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);

					BossKraid* bossKraid = static_cast<BossKraid*>(i->object);
				
					if (bossKraid->isActivitied())
					{
						this->isCollided = true;
						this->velocity = VECTOR2ZERO;
						
						this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);

						bossKraid->setBeHit(true);
						bossKraid->decreaseHealth(this->dame);
					}
					break;
				}
				case eID::MOTHERBRAIN:
				{
					Sound::getInstance()->stop(SOUND_BOMB_BURST);
					Sound::getInstance()->play(SOUND_BOMB_BURST, false);

					MotherBrain* motherBrain = static_cast<MotherBrain*>(i->object);
					
					if (motherBrain->isActivitied())
					{
						this->isCollided = true;
						this->sprite->setData(IndexManager::getInstance()->samusPinkExplosion[0]);
						this->velocity = VECTOR2ZERO;
						motherBrain->setBeHit(true);
						motherBrain->decreaseHealth(this->dame);
					}
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
	Sound::getInstance()->play(SOUND_ROCKET, false);

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


