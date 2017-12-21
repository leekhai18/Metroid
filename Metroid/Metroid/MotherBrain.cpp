#include "MotherBrain.h"

#define NUMBER_BULLET 10
#define WIDTH_BULLET 50
#define RATE_BEZIER 1.0f

#define DISTANCE_TO_FIRE 150
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

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->motherBrain, NUM_FRAMES_MOTHER, 0.1f);
	this->anim->start();
	this->getSprite()->setOrigin(VECTOR2(0.5, 0.5));


	isActivity = false;
	this->samus = samus;
	
	bulletPool = new MBulletPool(textureM, graphics, samus, NUMBER_BULLET);
}


MotherBrain::~MotherBrain()
{
	delete this->sprite;
	delete anim;
	delete bulletPool;
}

void MotherBrain::initStartBulletPool(VECTOR2 position)
{	
	bulletPool->setOwnPosition(position);
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
	
	if(isActivity)
	{
		bulletPool->handleVelocity(dt);
	}
	else
	{
		if (samus->getPosition().x - this->getPosition().x <= DISTANCE_TO_FIRE)
		{
			isActivity = true;
			initStartBulletPool(this->getPosition());
		}
	}
}

void MotherBrain::onCollision(Samus * samus, float dt)
{
	if (isActivity)
	{
		bulletPool->onCollision(dt);
	}
}



void MotherBrain::update(float dt)
{
	if (isActivity)
	{
		bulletPool->update(dt);
	}
	this->anim->update(dt);
	
}

void MotherBrain::draw()
{
	this->sprite->draw();
	bulletPool->draw();
}
