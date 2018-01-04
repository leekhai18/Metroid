#include "MotherBrain.h"
#include "Camera.h"
#define NUMBER_BULLET 10
#define WIDTH_BULLET 50
#define RATE_BEZIER 1.0f

#define DISTANCE_TO_FIRE 150
#define TIME_DELAY_BE_HIT 0.2f
#define TIME_BEFORE_DEATH 6.0f
#define CYCLE 0.1
MotherBrain::MotherBrain()
{
}

MotherBrain::MotherBrain(TextureManager * textureM, Graphics * graphics,Samus* samus) : BaseObject(eID::MOTHERBRAIN)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite MotherBrain");
	}

	this->initExplosion(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY);

	//this->anim = new Animation(this->sprite, IndexManager::getInstance()->motherBrain, NUM_FRAMES_MOTHER, 0.1f);
	fullHeath= new Animation(this->sprite, IndexManager::getInstance()->motherBrain, NUM_FRAMES_MOTHER, 0.1f);
	openingBoss = new Animation(this->sprite, IndexManager::getInstance()->motherBrainOpening, NUM_FRAMES_MOTHER, 0.1f);
	openedBoss= new Animation(this->sprite, IndexManager::getInstance()->motherBrainOpened, 2, 0.1f);
	this->anim = fullHeath;
	this->anim->start();
	this->getSprite()->setOrigin(VECTOR2(0.5, 0.5));


	isActivity = true;
	this->samus = samus;
	health = 50;
	bulletPool = new MBulletPool(textureM, graphics, samus, NUMBER_BULLET);
	this->isHandle = true;
}


MotherBrain::~MotherBrain()
{
	delete this->sprite;
	delete fullHeath;
	delete openingBoss;
	delete openedBoss;
	delete bulletPool;
}

void MotherBrain::initStartBulletPool(VECTOR2 position)
{	
	bulletPool->setOwnPosition(position);
}

void MotherBrain::setPort(Port * port)
{
	this->port = port;
}

void MotherBrain::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->getSprite()->getWidth() *0.5f ;
	rect.right = position.x + this->getSprite()->getWidth() *0.5f ;
	rect.top = position.y + this->getSprite()->getHeight() *0.5f ;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f ;

	this->boundCollision = rect;
}

void MotherBrain::reInit()
{
	
}

void MotherBrain::handleVelocity(float dt)
{
	
	/*if(isHandle)
	{
		bulletPool->handleVelocity(dt);
	}
	else
	{
		if (samus->getPosition().x - this->getPosition().x <= DISTANCE_TO_FIRE)
		{
			isHandle = true;
			initStartBulletPool(this->getPosition());
		}
	}*/
}

void MotherBrain::onCollision(Samus * samus, float dt)
{
	/*if (isHandle&&isActivity)
	{
		bulletPool->onCollision(dt);
	}*/
}


void MotherBrain::update(float dt)
{
	if (isActivity)
	{
		if (beHit)
		{
			timerHit += dt;
			if (timerHit < TIME_DELAY_BE_HIT)
			{
				this->anim->setPause(true);
				this->setVelocity(VECTOR2(0, 0));
			}
			else
			{
				timerHit = 0;
				beHit = false;
				this->anim->setPause(false);
				
				if (this->health <= 0)
				{

					this->isHandle = false;
					this->anim = this->openedBoss;
					this->anim->start();
					position = Camera::getInstance()->getPosition();
				}

				if (this->health <= 30&& this->health > 0)
				{
					this->anim = this->openingBoss;
					this->anim->start();
					//position = Camera::getInstance()->getPosition();
				}
			}
		}

	
		this->anim->update(dt);
		if (isHandle == false)
		{
			timeBeforDeath += dt;

			if (timeBeforDeath >= CYCLE)
			{
				if (cycle%2 == 0)
				{
					Camera::getInstance()->setPosition(VECTOR2(position.x, position.y + 8));
				}
				else
				{
					Camera::getInstance()->setPosition(VECTOR2(position.x, position.y - 8));
				}
				++cycle;
			}

			if (timeBeforDeath >= TIME_BEFORE_DEATH)
			{
				isActivity = false;
				this->port->setWin(true);
				Camera::getInstance()->setPosition(position);
			}

		}
	}
	
	
}

void MotherBrain::draw()
{
	if (isActivity)
	{
		this->sprite->draw();
	}
}
void MotherBrain::setBeHit(bool hit)
{
	this->beHit = hit;
}
void MotherBrain::decreaseHealth(float dame)
{
	this->health = this->health - dame;
}
