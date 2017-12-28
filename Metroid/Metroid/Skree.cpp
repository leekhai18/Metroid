#include "Skree.h"
#include "ObjectManager.h"
#include "Collision.h"
#include "Camera.h"
#include "GameLog.h"
#include "SamusStateManager.h"

#define TIME_FRAME_DELAY 0.15f
#define AREA_ACTIVE_X 40
#define TIME_FRAME_DELAY_FALLING 0.05f
#define EFFECT_DEATH_TIME 0.5f
#define VELOCITY_X 80
#define VELOCITY_Y 130
#define OFFSET_FOLLOW 2
#define TIME_DELAY_BE_HIT 0.2f
#define TIME_RETURN_NOMAL 5.0f

Skree::Skree(TextureManager * textureM, Graphics * graphics, EnemyColors color) : 
	BaseObject(eID::SKREE), IFreezable(IndexManager::getInstance()->skreeBlue)
	
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Skree");
	}

	this->initExplosion(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY);

	this->initItem(this->sprite, IndexManager::getInstance()->en, NUM_FRAMES_BONUS, TIME_FRAME_DELAY);

	this->sprite->setOrigin(VECTOR2(0.5f, 0));

	beHit = false;
	timerHit = 0;
	//this->explosion = new Animation(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY, false);

	effectDeath = new SkreeEffectDeath(textureM, graphics);
	timerDeath = 0;

	switch (color)
	{
	case Yellow:
		animationRotate = new Animation(this->sprite, IndexManager::getInstance()->skreeYellow, NUM_FRAMES_SKREE, TIME_FRAME_DELAY);
		health = 2;
		break;

	case Brown:
		animationRotate = new Animation(this->sprite, IndexManager::getInstance()->skreeBrown, NUM_FRAMES_SKREE, TIME_FRAME_DELAY);
		health = 2;
		break;

	default:
		break;
	}
	this->listWallCanCollide = new map<int,BaseObject*>();
	this->listCollide = new list<CollisionReturn>();
	animationRotate->start();

	target = VECTOR2ZERO;

	isActivity = true;
	isExplose = false;
	isHandle = true;
}

Skree::Skree()
{
}


Skree::~Skree()
{
	delete this->effectDeath;
	delete listWallCanCollide;
	delete listCollide;
	release();
}

bool Skree::checkCollision(Samus * sam, float dt)
{
	return this->effectDeath->checkCollision(sam, dt);
}

void Skree::onCollision(Samus* sam) // handle collide with skree's bullet
{
	if (!sam->isInStatus(eStatus::INJURING))
	{
		SamusStateManager::getInstance()->setOldStatus(sam->getStatus());
		sam->setStatus(eStatus::INJURING);
		SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getCurrentState());

		GAMELOG("Skree's bullets hit");
	}
}

void Skree::handleVelocity(float dt)
{
	if (isActivity&&isHandle&&!this->isCold)
	{
		if (this->target != VECTOR2ZERO && this->isInStatus(eStatus::START)) // check init
		{
			/*if(abs(target.x - initPosition.x) <= 1)
			{*/
				this->setStatus(eStatus::FALLING);
				this->animationRotate->setTimeFrameDelay(TIME_FRAME_DELAY_FALLING);
				this->setVelocityY(-VELOCITY_Y);
			//}

		}

		if (this->isInStatus(eStatus::FALLING))
		{
			if (target.x + OFFSET_FOLLOW < this->getPosition().x) // on left side
				this->setVelocityX(-VELOCITY_X);
			if (target.x - OFFSET_FOLLOW > this->getPosition().x)
				this->setVelocityX(VELOCITY_X);
			if (this->getPosition().x > target.x - OFFSET_FOLLOW && this->getPosition().x < target.x + OFFSET_FOLLOW) // in bound offset
				this->setVelocityX(0);

			
		}
	}
}

void Skree::onCollision(float dt)
{
	for (auto i = this->listWallCanCollide->begin(); i != this->listWallCanCollide->end(); i++)
	{
		BaseObject* x = (*i).second;
		Collision::getInstance()->checkCollision(this, x, dt);
	}
	if (isActivity&&isHandle && !this->isCold)
	{
		for (auto i = listCollide->begin(); i != listCollide->end(); i++)
		{
			switch (i->direction)
			{
			case CollideDirection::TOP:
				this->setStatus(eStatus::ENDING);
				this->velocity = VECTOR2ZERO;
				break;
			default:
				break;
			}
		}
		this->listCollide->clear();
	}


}

void Skree::update(float dt)
{
	//call this  in object class and set cold to true in bullet class 
	

	if (isActivity&&isHandle)
	{
		if (this->isCold)
		{
			timeReturnNormal += dt;
			if (timeReturnNormal >= TIME_RETURN_NOMAL)
			{
				this->animationRotate->setPause(false);
				this->isCold = false;
			}
			else
			{
				this->sprite->setData(this->frameID[animationRotate->getCurrentFrame()]);
				this->animationRotate->setPause(true);
				this->setVelocity(VECTOR2(0, 0));
				return;
			}

		}
		if (timerDeath < EFFECT_DEATH_TIME)
		{
			this->animationRotate->update(dt);
		}

		

		if (this->isInStatus(eStatus::ENDING))
		{
			if (timerDeath < EFFECT_DEATH_TIME)
				timerDeath += dt;
			else
			{
				if (this->canDraw)
				{
					this->effectDeath->init(VECTOR2(this->getPosition().x, this->getPosition().y - this->getSprite()->getHeight()*0.4f));
					this->canDraw = false;
				}

				this->effectDeath->update(dt);

				if (this->effectDeath->isFinished())
				{
					this->finish();
					isHandle = false;
					//target = VECTOR2ZERO;
					//isCold = false;
					//IBonusable::start();
				}
					
			}
		}


		if (beHit)
		{
			timerHit += dt;
			if (timerHit < TIME_DELAY_BE_HIT)
			{
				//this->animationRotate->setPause(true);
				this->setVelocity(VECTOR2(0, 0));
			}
			else
			{
				timerHit = 0;
				beHit = false;
				this->animationRotate->setPause(false);
				this->setVelocityY(-VELOCITY_Y);

				if (this->health <= 0)
				{
					this->finish();
					//isReleaseBullet = true;
					IExplosible::start();
					isHandle = false;
					//target = VECTOR2ZERO;
					//isCold = false;
				}
			}
		}
		this->setPosition(this->getPosition().x + this->getVelocity().x*dt, this->getPosition().y + this->getVelocity().y*dt);
		setBoundCollision();
	}

	if (!Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->startBound)
		&& !Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->boundCollision))
	{
		reInit();
	}
	if (isExplose)
	{
		IBonusable::update(dt);
		//target = VECTOR2ZERO;
	}
	else
	{
		IExplosible::update(dt);
		if(isExplose)
		{
			IBonusable::start();
		}
	}
	
}

void Skree::draw()
{
	if (isActivity)
		this->effectDeath->draw();

	if (this->canDraw)
		this->sprite->draw();
}


void Skree::release()
{
	delete this->animationRotate;
	delete this->sprite;
}

void Skree::setStartBound(MetroidRect rect)
{
	this->startBound = rect;
}


bool Skree::getHandle()
{
	return isHandle;
}

VECTOR2 Skree::getTarget()
{
	return this->target;
}

void Skree::setTarget(VECTOR2 target)
{
	if (isActivity)
	{
		if (!this->isInStatus(eStatus::ENDING))
		{
			if (abs(target.x - this->getPosition().x) < AREA_ACTIVE_X)
			{
				this->target = target;
				
			}
		}
	}

}

void Skree::setBoundCollision()
{
	boundCollision.left = this->getPosition().x - this->getSprite()->getWidth()*0.5f;
	boundCollision.top = this->getPosition().y;
	boundCollision.right = boundCollision.left + this->getSprite()->getWidth();
	boundCollision.bottom = boundCollision.top - this->getSprite()->getHeight();
}

void Skree::setInitPosition(VECTOR2 pos)
{
	this->initPosition = pos;
	this->setPosition(pos);
}

void Skree::reInit()
{
	canDraw = true;
	isActivity = true;
	isHandle = true;
	isCold = false;
	this->animationRotate->setPause(false);
	this->explosion->setPause(false);
	//this->explosion->setPause(false);
	this->setPosition(initPosition);
	this->setStatus(eStatus::START);
	this->target = VECTOR2ZERO;
	timerDeath = 0;
	this->animationRotate->setTimeFrameDelay(TIME_FRAME_DELAY);
	this->velocity = VECTOR2ZERO;
	this->health = 2;
	this->explosion->reInit();
	timeReturnNormal = 0;
	isExplose = false;
}

void Skree::finish()
{
	isHandle = false;
	this->velocity = VECTOR2ZERO;
	this->effectDeath->setPosition(VECTOR2ZERO);
}

void Skree::setBeHit(bool flag)
{
	beHit = flag;
}

void Skree::decreaseHealth(float dame)
{
	this->health = this->health - dame;
}

map<int,BaseObject*>* Skree::getListWallCanCollide()
{
	return this->listWallCanCollide;
}

list<CollisionReturn>* Skree::getListCollide()
{
	return this->listCollide;
}
