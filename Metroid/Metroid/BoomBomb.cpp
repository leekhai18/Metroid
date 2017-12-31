#include "BoomBomb.h"
#include "BoomBombPool.h"
#include "Collision.h"
#include "GameLog.h"
#include "Skree.h"
#include "Zommer.h"
#include "Waver.h"
#include "Rio.h"
#include "Zeb.h"
#include "Brick.h"
#include "BossKraid.h"
#include "MotherBrain.h"
#define TIME_BOM 2
#define W_H_BOOM_HALF 12

BoomBomb::BoomBomb(TextureManager * textureM, Graphics * graphics, Samus* samus) : BaseObject(eID::BOOMBOMB)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite BOOMBOMB");
	}
	this->samus = samus;
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->explosion = new Animation(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY, false);
	this->anim = new Animation(this->sprite, IndexManager::getInstance()->samusYellowBombEffect, NUM_FRAMES_SAMUS_BOMB, 0.1f);

	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);

	this->timer = 0;
	this->listCollide = new list<BaseObject*>();

	this->dame = 2; // se setup lai sau
}

void BoomBomb::setSamus(Samus * samus)
{
	this->samus = samus;
}

BoomBomb::BoomBomb()
{
}


BoomBomb::~BoomBomb()
{
	delete this->sprite;
	delete this->anim;
	delete this->explosion;
	delete this->listCollide;
}

void BoomBomb::onCollision()
{
	if (canHandledCollision)
	{
		for (auto i = listCollide->begin(); i != listCollide->end(); i++)
		{
			switch ((*i)->getId())
			{
			case eID::BRICK:
			{
				Brick* brick = static_cast<Brick*>(*i);
				brick->setActivity(false);
			}
			break;
			case eID::SKREE:
			{
				Skree* skr = static_cast<Skree*>(*i);
				/*if (BulletPool::getInstance()->getCurrentIceBullet())
				{
					skr->setCold(true);
				}*/
				skr->setBeHit(true);
				skr->decreaseHealth(this->dame);
				this->velocity = VECTOR2ZERO;
				break;
			}


			case eID::ZOMMER:
			{
				Zommer* zommer = static_cast<Zommer*>((*i));
				/*if (BulletPool::getInstance()->getCurrentIceBullet())
				{
					zommer->setCold(true);
				}*/
				this->velocity = VECTOR2ZERO;
				zommer->setBeHit(true);
				zommer->decreaseHealth(this->dame);
				break;
			}
			case eID::WAVER:
			{
				Waver* waver = static_cast<Waver*>(*i);

				waver->setBeHit(true);
				waver->decreaseHealth(this->dame);
				this->velocity = VECTOR2ZERO;
				break;
			}
			case eID::ZEB:
			{
				Zeb* zeb = static_cast<Zeb*>(*i);

				zeb->setBeHit(true);
				zeb->decreaseHealth(this->dame);
				this->velocity = VECTOR2ZERO;
				break;
			}
			case eID::BOSSKRAID:
			{
				BossKraid* bossKraid = static_cast<BossKraid*>(*i);

				bossKraid->setBeHit(true);
				bossKraid->decreaseHealth(this->dame);
				this->velocity = VECTOR2ZERO;
				break;
			}
			default:
				break;
			}
		}
		MetroidRect bound = samus->getBoundCollision();
		if (Collision::getInstance()->isCollide(bound, this->boundCollision))
		{
			if (this->boundCollision.bottom >= bound.bottom)
			{

				if (bound.right < this->boundCollision.left - 2)
				{
					samus->setBoomVelocity(VECTOR2(-100, 100));
					samus->setVelocity(VECTOR2(-100, 100));
					samus->setBoomExplose(true);
				}
				else if (bound.left > this->boundCollision.left + 2)
				{
					samus->setBoomVelocity(VECTOR2(100, 100));
					samus->setVelocity(VECTOR2(100, 100));
					samus->setBoomExplose(true);
				}
			}
			else
			{
				samus->setBoomVelocity(VECTOR2(this->samus->getVelocity().x, 100));
				samus->setVelocity(VECTOR2(100, 100));
				samus->setBoomExplose(true);
			}
		}
		else
		{
			samus->setBoomExplose(false);
		}
		canHandledCollision = false;
		
	}

	this->listCollide->clear();
}


void BoomBomb::update(float dt)
{
	if (!this->anim->isFinished())
	{
		timer += dt;
		if (timer > TIME_BOM)
		{
			this->anim->stop();
			this->explosion->start();
			this->setBoundCollision();
			this->canHandledCollision = true;
		}

		this->anim->update(dt);
	}
	else
	{
		this->explosion->update(dt);

		if (this->explosion->isFinished())
			BoomBombPool::getInstance()->returnPool(this);
	}
}

void BoomBomb::draw()
{
	if (!this->explosion->isFinished())
		this->sprite->draw();
}

void BoomBomb::setBoundCollision()
{
	this->boundCollision.top = this->getPosition().y + W_H_BOOM_HALF;
	this->boundCollision.bottom = this->getPosition().y - W_H_BOOM_HALF;
	this->boundCollision.left = this->getPosition().x - W_H_BOOM_HALF;
	this->boundCollision.right = this->getPosition().x + W_H_BOOM_HALF;
}

void BoomBomb::start(VECTOR2 pos)
{
	this->setPosition(pos);
	this->anim->start();
	this->explosion->reInit();
	this->timer = 0;
	this->setStatus(eStatus::START);
	this->sprite->setData(IndexManager::getInstance()->samusYellowBombEffect[0]);
}

void BoomBomb::returnPool()
{
	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);
	this->boundCollision = MetroidRect(0, 0, 0, 0);
}

list<BaseObject*>* BoomBomb::getListCollide()
{
	return this->listCollide;
}

