#include "DefendBoss.h"



DefendBoss::DefendBoss(TextureManager * textureM, Graphics * graphics):BaseObject(eID::DEFENSEBOSS)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
	}
	this->frame = IndexManager::getInstance()->wallFrontMotherBrain;
	this->totalFrame = 4;
	this->sprite->setData(this->frame[0]);
	this->setOrigin(VECTOR2(0.5, 0.5));
	heath = 20;
	isActivity = true;
}

void DefendBoss::setBeHit(bool hit)
{
	this->beHit = true;
}

void DefendBoss::decreaseHealth(float dame)
{
	this->heath -= dame;
}

void DefendBoss::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);

	rect.left = position.x - this->getSprite()->getWidth()*0.5f;

	rect.right = position.x + this->getSprite()->getWidth()*0.5f;

	rect.top = position.y + this->getSprite()->getHeight()*0.5f;

	rect.bottom = position.y - this->getSprite()->getHeight()*0.5f;

	this->boundCollision = rect;
}

void DefendBoss::update(float dt)
{
	if(isActivity)
	{
		if(beHit)
		{
			if (heath <= 15 && heath > 10)
			{
				this->sprite->setData(this->frame[1]);
				setBoundCollision();
			}
			else if (heath <= 10 && heath > 5)
			{
				this->sprite->setData(this->frame[2]);
				setBoundCollision();
			}
			else if (heath <= 5 && heath > 0)
			{
				this->sprite->setData(this->frame[3]);
				setBoundCollision();
			}
			else if (heath <= 0)
			{
				isActivity = false;		
			}

			/*switch (heath)
			{
			case 15:
				this->sprite->setData(this->frame[1]);
				setBoundCollision();
				break;
			case 10:
				this->sprite->setData(this->frame[2]);
				setBoundCollision();
				break;
			case 5:
				this->sprite->setData(this->frame[3]);
				setBoundCollision();
				break;
			case 0:
				isActivity = false;
			default:
				break;
			}*/
			beHit = false;
		}
		
	}
	
}

void DefendBoss::draw()
{
	if (isActivity)
	{
		this->sprite->draw();
	}
}

DefendBoss::DefendBoss()
{
}


DefendBoss::~DefendBoss()
{
	delete sprite;
}
