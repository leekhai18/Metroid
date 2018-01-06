
#include "SamusStateInjuring.h"
#include "SamusStateManager.h"
#include "Sound.h"
#define TIME_ANIMATION 0.1f
#define TIME_TO_NORMAL 0.5f
SamusStateInjuring::SamusStateInjuring()
{
}
void SamusStateInjuring::setBoundCollision()
{
	SamusStateManager::getInstance()->getOldState()->setBoundCollision();
}
SamusStateInjuring::SamusStateInjuring(Samus * samus, Input * input) : BaseState(samus, input)
{
	
}

SamusStateInjuring::~SamusStateInjuring()
{

}

void SamusStateInjuring::init()
{
	Sound::getInstance()->stop(SOUND_ENEMIES_COLLISION);
	Sound::getInstance()->play(SOUND_ENEMIES_COLLISION, false);
	
	//this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);
	setBoundCollision();

	time_to_stand = 0;
	time_animation = 0;

	this->samus->setHealth(*this->samus->getHealth() - 8);
}

void SamusStateInjuring::handleInput(float dt)
{
	this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);
	switch(samus->getDirectCollide())
	{
	case CollideDirection::BOTTOM:
		this->samus->setVelocityX(SAMUS_VERLOCITY_X*this->samus->getDirection());
		break;
	case CollideDirection::TOP:
		this->samus->setVelocityX(SAMUS_VERLOCITY_X*this->samus->getDirection());
		break;
	case CollideDirection::RIGHT:
		this->samus->setVelocityX(SAMUS_VERLOCITY_X);
		break;
	case CollideDirection::LEFT:
		
		this->samus->setVelocityX(-SAMUS_VERLOCITY_X);
		break;
	}
	
}

void SamusStateInjuring::onCollision(float dt)
{
	MetroidRect bound;
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
		case eID::WALL:
		case eID::BRICK:
		case eID::FIRE:
		case eID::ELEVATOR:
		case eID::PORT:
		case eID::GATEBLUE:
		case eID::GATERED:
		case eID::DEFENSEBOSS:
			switch (i->direction)
			{
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);
				if (SamusStateManager::getInstance()->getOldStatus() == eStatus::RUNNING ||
					SamusStateManager::getInstance()->getOldStatus() == eStatus::STANDING)
				{
					this->samus->setPositionY(i->positionCollision + OFFSET_STAND);
					//this->samus->setStatus(eStatus::STANDING);
				}
				else if(SamusStateManager::getInstance()->getOldStatus() == eStatus::JUMPING)
				{
					this->samus->setPositionY(i->positionCollision + OFFSET_STAND);
				}
				if(SamusStateManager::getInstance()->getOldStatus() == eStatus::ROLLING || 
					SamusStateManager::getInstance()->getOldStatus() == eStatus::FALLING_ROLLING)
				{
					this->samus->setPositionY(i->positionCollision + OFFSET_ROLLING);
				}
				break;
			case CollideDirection::LEFT:

				if (!(this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top))
				{
					this->samus->setVelocityY(0);
					break;
				}
				//}
				this->samus->setVelocityX(0);
				break;
			case CollideDirection::RIGHT:

				if (!(this->samus->getBoundCollision().bottom< i->object->getBoundCollision().top))
				{
						this->samus->setVelocityY(0);
						break;
				}
				//}
				this->samus->setVelocityX(0);
				break;
			case CollideDirection::BOTTOM:
				this->samus->setVelocityY(0);
				break;
			}
			break;

		case eID::ZOMMER:
		case eID::SKREE:
		case eID::RIO:
		case eID::RIPPER:
		case eID::WAVER:
		case eID::ZEB:
		case eID::BOSSKRAID:
		case eID::MOTHERBRAIN:
			switch (i->direction)
			{
			case CollideDirection::TOP:
				
				this->samus->setVelocityY(0);

				break;
			case CollideDirection::LEFT:
				
					if (!(this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top))
					{
						this->samus->setVelocityY(0);
						break;
					}
				
				this->samus->setVelocityX(0);
				break;
			case CollideDirection::RIGHT:

					if (!(this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top))
					
					{
						this->samus->setVelocityY(0);
						break;
					}
				
				this->samus->setVelocityX(0);
				break;
			case CollideDirection::BOTTOM:
				this->samus->setVelocityY(0);
				break;
			}
			//this->samus->setVelocity(VECTOR2(0, 0));
			break;
		default:
			break;
		}
	}
}

void SamusStateInjuring::update(float dt)
{
	time_to_stand += dt;
	time_animation += dt;
	this->samus->updateVertical(dt);
	this->samus->updateHorizontal(dt);
	setBoundCollision();

	if (time_animation >= TIME_ANIMATION)
	{
		if (this->samus->getVisible())
		{
			this->samus->setVisible(false);
		}
		else
		{
			this->samus->setVisible(true);
		}
		time_animation = time_animation - TIME_ANIMATION;
	}

	if(time_to_stand >= TIME_TO_NORMAL)
	{
		this->samus->setStatus(SamusStateManager::getInstance()->getOldStatus());
	}

	if (this->samus->getStatus() != eStatus::INJURING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::ROLLING:
			this->samus->setVelocityX(0);
			break;
		default:
			break;
		}
		this->samus->setVisible(true);
		this->samus->setCanMoveLeft(true);
		this->samus->setCanMoveRight(true);
		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}
}

void SamusStateInjuring::onStart()
{
}

void SamusStateInjuring::onExit()
{
}

void SamusStateInjuring::setOldStatus(eStatus status)
{
	this->oldStatus = status;
}

eStatus SamusStateInjuring::getOldStatus()
{
	return this->oldStatus;
}


