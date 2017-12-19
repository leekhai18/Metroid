#include "BoomBomb.h"
#include "BoomBombPool.h"
#include "Collision.h"
#include "GameLog.h"
#include "Skree.h"
#include "Zommer.h"

#define TIME_BOM 2
#define W_H_BOOM_HALF 8

BoomBomb::BoomBomb(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::BOOMBOMB)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite BOOMBOMB");
	}
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->samusYellowBombEffect, NUM_FRAMES_SAMUS_BOMB, 0.1f);

	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);

	this->timer = 0;
	this->listCollide = new list<CollisionReturn>();

	this->dame = 1; // se setup lai sau
}

BoomBomb::BoomBomb()
{
}


BoomBomb::~BoomBomb()
{
	delete this->sprite;
	delete this->anim;
	this->listCollide->clear();
	delete this->listCollide;
}

void BoomBomb::onCollision()
{
	for (auto i = this->listCollide->begin(); i != this->listCollide->end(); i++)
	{
		switch (i->object->getId())
		{
			case eID::SKREE:
			{
				Skree* skr = static_cast<Skree*>(i->object);
				skr->setBeHit(true);
				skr->decreaseHealth(this->dame);
				break;
			}

			case eID::ZOMMER:
			{
				Zommer* zommer = static_cast<Zommer*>((*i).object);
				zommer->setCold(true);
				break;
			}
		}
	}

	this->listCollide->clear();
}


void BoomBomb::update(float dt)
{
	timer += dt;
	if (timer > TIME_BOM)
	{
		this->anim->stop();
		BoomBombPool::getInstance()->returnPool(this);
	}
	else
	{
		this->anim->update(dt);
	}
}

void BoomBomb::draw()
{
	if (!this->anim->isFinished())
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
	this->timer = 0;
	this->setStatus(eStatus::START);
}

void BoomBomb::returnPool()
{
	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);
}

list<CollisionReturn>* BoomBomb::getListCollide()
{
	return this->listCollide;
}

