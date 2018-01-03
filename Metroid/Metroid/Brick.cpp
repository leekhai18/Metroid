#include "Brick.h"
#include "Collision.h"
#define TIME_TO_RESET 5.0f

Brick::Brick()
{
}

Brick::Brick(TextureManager * textureM, Graphics * graphics, BrickStyle style) : BaseObject(eID::BRICK)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Brick");
	}

	switch (style)
	{
	case BrickSecretBlue:
		this->sprite->setData(IndexManager::getInstance()->brickSecretBlue);
		break;
	case BrickSerectGreen:
		this->sprite->setData(IndexManager::getInstance()->brickSecretGreen);
		break;
	case BrickGreen:
		this->sprite->setData(IndexManager::getInstance()->brickGreen);
		break;
	case BrickBlue:
		this->sprite->setData(IndexManager::getInstance()->brickBlue);
		break;
	case BrickRed:
		this->sprite->setData(IndexManager::getInstance()->brickRed);
		break;
	default:
		break;
	}
	isActivity = true;
	visible = true;
	listCanCollide = new map<int,BaseObject*>();
	heath = 8;
}


Brick::~Brick()
{
	delete this->sprite;
	delete listCanCollide;
}

bool Brick::getVisible()
{
	return this->visible;
}

void Brick::decreaseHealth(float dame)
{
	this->heath -= dame;
}




void Brick::setVisible(bool visible)
{
	this->visible = visible;
}

void Brick::onCollision(float dt)
{
	if (!this->isActivity&&visible)
	{
		for (auto i = listCanCollide->begin(); i !=listCanCollide->end(); i++)
		{
			BaseObject* object = (*i).second;
			if(Collision::getInstance()->isCollide(this->boundCollision, object->getBoundCollision()))
			{
				this->timeReset = 0;
			}
		}
	}
}

void Brick::update(float dt)
{

	if (!this->isActivity&&visible)
	{
		this->timeReset += dt;
		if (timeReset >= TIME_TO_RESET)
		{
			timeReset = 0;
			isActivity = true;
			heath = 8;
		}
		
	}
	if(this->isActivity&&visible)
	{
		if (heath <= 0)
		{
			this->isActivity = false;
		}
	}
	
}

void Brick::draw()
{
	if (this->isActivity&&visible)
		this->sprite->draw();
}


