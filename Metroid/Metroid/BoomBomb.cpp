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

	this->explosion = new Animation(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY, false);
	this->anim = new Animation(this->sprite, IndexManager::getInstance()->samusYellowBombEffect, NUM_FRAMES_SAMUS_BOMB, 0.1f);

	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);

	this->timer = 0;
	this->listCollide = new list<BaseObject*>();

	this->dame = 1; // se setup lai sau
}

BoomBomb::BoomBomb()
{
}


BoomBomb::~BoomBomb()
{
	delete this->sprite;
	delete this->anim;
	delete this->explosion;
	this->listCollide->clear();
	delete this->listCollide;
}

void BoomBomb::onCollision()
{
	if (canHandledCollision)
	{
		for (auto i = listCollide->begin(); i != listCollide->end(); i++)
		{
			GAMELOG("BOOOOMM");
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

