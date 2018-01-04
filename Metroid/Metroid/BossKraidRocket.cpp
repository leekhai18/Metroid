#include "BossKraidRocket.h"
#include "Collision.h"
#include "BossKraid.h"
#include "Samus.h"
#include <math.h>
#include "BossRocketPool.h"
#include "SamusStateManager.h"
#define ROCKET_VELOCITY_X 100
#define ROCKET_VELOCITY_Y 100
#define ROCKET_VELOCITY VECTOR2(ROCKET_VELOCITY_X,ROCKET_VELOCITY_Y)
BossKraidRocket::BossKraidRocket()
{
}

BossKraidRocket::BossKraidRocket(Graphics * graphics, TextureManager * textureM, Samus * samus, BossKraid* boss):BaseObject(eID::BOSSKRAID_ROCKET)
{
	this->samus = samus;
	this->boss = boss;

	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bomb");
	}
	listCollide = new list<CollisionReturn>();

	this->initExplosion(sprite, IndexManager::getInstance()->explosionRocketBoss, NUM_ROCKET_BOSS_EXPLOSE, EXPLOSION_TIME_FRAME_DELAY);
	//this->anim = new Animation(this->sprite, IndexManager::getInstance()->explosionRocketBoss, NUM_ROCKET_BOSS_EXPLOSE, TIME_FRAME_ITEM);
	//this->anim->start();
	this->sprite->setData(IndexManager::getInstance()->rocketBoss);
	this->isActivity = true;

	this->setOrigin(VECTOR2(0.5, 0.5));
	this->status = eStatus::ENDING;
	dame = 5;
	isReInit = true;
}

void BossKraidRocket::init()
{
	if (boss->getDirection() == eDirection::right)
	{
		P1 = VECTOR2(boss->getPosition().x + 1, boss->getPosition().y) - boss->getPosition();
		P2 = samus->getPosition() - boss->getPosition();
		angle = acos(D3DXVec2Dot(&P1, &P2) / (D3DXVec2Length(&P1)*D3DXVec2Length(&P2)));
		this->setPosition(boss->getBoundCollision().left, boss->getBoundCollision().top - 5);
		if(P1.y>=P2.y)
		{
			this->sprite->setRotate(angle*(180.0f / 3.14f));
		
		}
		else
		{
			this->sprite->setRotate(360 - angle*(180.0f / 3.14f));
		}
		this->sprite->setFlipX(false);
	}
	else
	{
		P1 = boss->getPosition() - VECTOR2(boss->getPosition().x - 1, boss->getPosition().y);
		P2 = samus->getPosition()- boss->getPosition();
		angle = acos(D3DXVec2Dot(&P1, &P2) / (D3DXVec2Length(&P1)*D3DXVec2Length(&P2)));

		this->setPosition(boss->getBoundCollision().left, boss->getBoundCollision().top - 5);

		if (P1.y >= P2.y)
		{
			this->sprite->setRotate(angle*(180.0f / 3.14f));
			
		}
		else
		{
			
			this->sprite->setRotate(360 - angle*(180.0f / 3.14f));

		}

		//this->sprite->setFlipX(true);
	} 

	D3DXVec2Normalize(&normalize, &P2);

	this->status = eStatus::START;

	setBoundCollision();

	IExplosible::reInit();

	this->isActivity = true;

	this->sprite->setData(IndexManager::getInstance()->rocketBoss);

	this->isActivity = true;
}

void BossKraidRocket::returnPool()
{
	this->setStatus(eStatus::ENDING);
	this->setPosition(VECTOR2ZERO);
	setBoundCollision();

	isCollided = false;
	this->timer = 0;
	this->isActivity = false;
	this->velocity = VECTOR2ZERO;
}
void BossKraidRocket::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->getSprite()->getWidth() *0.5f;
	rect.right = position.x + this->getSprite()->getWidth() *0.5f;
	rect.top = position.y + this->getSprite()->getHeight() *0.5f;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f;


	this->boundCollision = rect;

}
void BossKraidRocket::setListWallCanCollide(map<int, BaseObject*>* list)
{
	this->listWallCanCollide = list;
}
map<int, BaseObject*>* BossKraidRocket::getListWallCanCollide()
{
	return this->listWallCanCollide;
}
list<CollisionReturn>* BossKraidRocket::getListCollide()
{
	return this->listCollide;
}

void BossKraidRocket::handleVelocity(float dt)
{
	if (isActivity)
	{
		if (this->status == eStatus::START)
		{
			this->velocity = VECTOR2(ROCKET_VELOCITY_X*normalize.x, ROCKET_VELOCITY_Y*normalize.y);
			this->status = eStatus::RUNNING;
		}
	}
}
void BossKraidRocket::onCollisionSamus(float dt)
{

	if (Collision::getInstance()->checkCollision(this, samus, dt))
	{
		this->status = eStatus::ENDING;
		this->velocity = VECTOR2ZERO;
		if(!samus->isInStatus(eStatus::INJURING))
		{
			SamusStateManager::getInstance()->setOldStatus(samus->getStatus());
			samus->setStatus(eStatus::INJURING);
			samus->setHealth(samus->getHealth() - dame);
			SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getCurrentState());
		}
	
	}
}
void BossKraidRocket::onCollision(float dt)
{
	if (isActivity)
	{
		if (listWallCanCollide)
		{
			for (auto i = this->listWallCanCollide->begin(); i != this->listWallCanCollide->end(); i++)
			{
				BaseObject* x = (*i).second;
				Collision::getInstance()->checkCollision(this, x, dt);
			}
		}
		if(this->listCollide->size() > 0)
		{
			this->status = eStatus::ENDING;
			this->velocity = VECTOR2ZERO;
		}
		this->listCollide->clear();
	}
}

void BossKraidRocket::update(float dt)
{
	if (isActivity)
	{
		if(this->status == eStatus::ENDING)
		{
			IExplosible::start();
			isHandle = false;
		}
		else
		{
			this->setPosition(VECTOR2(this->getPosition().x + dt*velocity.x, this->getPosition().y + dt*velocity.y));
			setBoundCollision();
		}
	}
	if (this->status == eStatus::ENDING)
	{
		IExplosible::update(dt);
	}	
	if(isExplose)
	{
		BossRocketPool::getInstance()->returnPool(this);
	}
}

void BossKraidRocket::draw()
{
	this->sprite->draw();
}

BossKraidRocket::~BossKraidRocket()
{
	//delete this->anim;
	delete this->sprite;
	delete this->listCollide;
}
