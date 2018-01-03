#include "SamusStateRolling.h"
#include "SamusStateManager.h"
#include "Camera.h"
#include "GameLog.h"
#include "BoomBombPool.h"
#include "GateBlue.h"
#include "GateRed.h"
#include "Brick.h"

#include "Skree.h"
#include "Zommer.h"
#include "Waver.h"
#include "Zeb.h"
#include "Rio.h"

#include "Sound.h"
#define TIME_START_BOMB 0.1f
SamusStateRolling::SamusStateRolling()
{
}

SamusStateRolling::SamusStateRolling(Samus * samus, Input * input) : BaseState(samus, input)
{
	timer = 0;
	timerToStartBoom = 0;
}


SamusStateRolling::~SamusStateRolling()
{

}

void SamusStateRolling::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y );
	rect.left = position.x - WIDTH_COLLISION*0.5f;
	rect.right = position.x + WIDTH_COLLISION*0.5f;
	rect.top = position.y + ROLL_HEIGHT*0.5f;
	rect.bottom = position.y - ROLL_HEIGHT*0.5f;
	samus->setBoundCollision(rect);
}

void SamusStateRolling::init()
{
	this->animation = samus->getRollingAnim();

	setBoundCollision();

	//this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);


	move_to_fall = true;
	canStanding = false;
}

void SamusStateRolling::handleInput(float dt)
{
	this->samus->setVelocity(VECTOR2(this->samus->getVelocity().x, -SAMUS_MIN_SPEED_Y));
	if (!Camera::getInstance()->moveWhenSamusOnPort()) {
		//handle press left button
		if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
		{
			if (this->samus->isInDirection(eDirection::right))
			{
				this->samus->setFlipX(true);
				this->samus->setDirection(eDirection::left);
			}

			//check if moveLeft = false, it means samus is colliding with other object in right side
			if (this->samus->canMoveLeft() == true)
			{
				this->samus->setVelocityX(-SAMUS_VERLOCITY_X);
			}
		}

		//handle press right button
		if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
		{

			if (this->samus->isInDirection(eDirection::left))
			{
				this->samus->setFlipX(false);
				this->samus->setDirection(eDirection::right);
			}

			//check if moveRight = false, it means samus is colliding with other object in left side
			if (this->samus->canMoveRight() == true)
			{
				this->samus->setVelocityX(SAMUS_VERLOCITY_X);
			}
		}

		if ((input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT)))
		{
			this->samus->setVelocityX(0);
		}

		if (input->isKeyDown(VK_UP) || input->isKeyDown(VK_X))
		{
			this->samus->setStatus(eStatus::STANDING);
		}
	}

	if (input->isKeyDown(VK_Z) && this->samus->isHaveBomb())
	{
		timerToStartBoom += dt;
		if (timerToStartBoom > TIME_START_BOMB)
		{
			Sound::getInstance()->play(SOUND_PUT_BOMB, false);
			BoomBombPool::getInstance()->getBoom()->start(this->samus->getPosition());
			timerToStartBoom = 0;
		}
	}
}

void SamusStateRolling::onCollision(float dt)
{
	if(this->samus->isInStatus(eStatus::STANDING) )
	{
		MetroidRect bound;
		bound.left = this->samus->getPosition().x - WIDTH_COLLISION*0.5f;
		bound.right = this->samus->getPosition().x + WIDTH_COLLISION*0.5f;
		bound.bottom = this->samus->getBoundCollision().bottom;
		bound.top = bound.bottom + 30;
		

		for (auto i = this->samus->getListCanCollide()->begin(); i != this->samus->getListCanCollide()->end(); ++i)
		{
			if (Collision::getInstance()->isCollide((*i).second->getBoundCollision(), bound))
			{
				if((*i).second->getId()==eID::BRICK)
				{
					Brick* brick = static_cast<Brick*>((*i).second);
					if(brick->getVisible())
					{
						this->samus->setStatus(eStatus::ROLLING);
					}
				}
				else
				{
					this->samus->setStatus(eStatus::ROLLING);
				}
			
			}
		}
	}
	MetroidRect bound;
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
		
#pragma region Wall			
		case eID::WALL:	
		
		case eID::ALIENBIG:
		case eID::ALIENSMALL:
			switch (i->direction)
			{
			case CollideDirection::LEFT:

				if (this->samus->getBoundCollision().bottom< i->object->getBoundCollision().top)
				{
					this->samus->setVelocityX(0);
					//not allow move left
					this->samus->setCanMoveRight(false);
				}

				break;
			case CollideDirection::RIGHT:

				if (this->samus->getBoundCollision().bottom< i->object->getBoundCollision().top)
				{
					this->samus->setVelocityX(0);
					//not allow move right
					this->samus->setCanMoveLeft(false);
				}
				break;
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);

				this->samus->setPositionY(i->positionCollision + OFFSET_ROLLING);
				canStanding = true;	
				move_to_fall = false;
				break;
			}
			break;
#pragma endregion
#pragma region Fire	
		case eID::FIRE:
		{
			switch (i->direction)
			{
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);

				this->samus->setPositionY(i->positionCollision + OFFSET_ROLLING);
				canStanding = true;
				move_to_fall = false;
				break;
			}
		}
#pragma endregion
#pragma region Elevator	
		case eID::ELEVATOR:
			if (samus->getCanMoveElevator())
			{
				return;
			}

			switch (i->direction)
			{
			case CollideDirection::LEFT:

				if (this->samus->getBoundCollision().bottom< i->object->getBoundCollision().top)
				{
					this->samus->setVelocityX(0);
					//not allow move left
					this->samus->setCanMoveRight(false);
				}

				break;
			case CollideDirection::RIGHT:

				if (this->samus->getBoundCollision().bottom< i->object->getBoundCollision().top)
				{
					this->samus->setVelocityX(0);
					//not allow move right
					this->samus->setCanMoveLeft(false);
				}
				break;
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);

				this->samus->setPositionY(i->positionCollision + OFFSET_ROLLING);
				canStanding = true;
				move_to_fall = false;
				break;
			}
			break;
#pragma endregion
		case eID::BRICK: 
		{
			Brick* brick = static_cast<Brick*>(i->object);
			if (brick->getVisible())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:

					if (this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top)
					{
						this->samus->setVelocityX(0);
						//not allow move left
						this->samus->setCanMoveRight(false);
					}

					break;
				case CollideDirection::RIGHT:

					if (this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top)
					{
						this->samus->setVelocityX(0);
						//not allow move right
						this->samus->setCanMoveLeft(false);
					}
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);

					this->samus->setPositionY(i->positionCollision + OFFSET_ROLLING);
					canStanding = true;
					move_to_fall = false;
					break;
				}

			}
			break; 
		}
#pragma region GATE and PORT
		case eID::GATEBLUE: case eID::GATERED:
		{
			if (i->object->isActivitied())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:
				{
					if (Camera::getInstance()->moveWhenSamusOnPort())
					{
						GateBlue* gate = static_cast<GateBlue*>(i->object);
						gate->setIsCollideSamusInPort(true);

						this->samus->setIsCollidingPort(false);
						this->samus->setVelocityX(0);
						this->samus->setCanMoveToFrontGate(true);
					}
					else
					{
						this->samus->setVelocityX(0);
						this->samus->setCanMoveRight(false);
					}

					break;
				}
				case CollideDirection::RIGHT:
				{
					if (Camera::getInstance()->moveWhenSamusOnPort())
					{
						GateBlue* gate = static_cast<GateBlue*>(i->object);
						gate->setIsCollideSamusInPort(true);

						this->samus->setIsCollidingPort(false);
						this->samus->setVelocityX(0);
						this->samus->setCanMoveToFrontGate(true);
					}
					else
					{
						this->samus->setVelocityX(0);
						this->samus->setCanMoveLeft(false);
					}

					break;
				}
				}
			}

			break;
		}

		case eID::PORT:
		{
			switch (i->direction)
			{
			case LEFT:
				Camera::getInstance()->setVelocity(VECTOR2(SAMUS_VERLOCITY_X, 0));
				Camera::getInstance()->setOnPort(true);
				this->animation->setPause(true);
				this->samus->setIsCollidingPort(true);
				break;
			case RIGHT:
				Camera::getInstance()->setVelocity(VECTOR2(-SAMUS_VERLOCITY_X, 0));
				Camera::getInstance()->setOnPort(true);
				this->animation->setPause(true);
				this->samus->setIsCollidingPort(true);
				break;
			default:
				break;
			}

			break;
		}

#pragma endregion

#pragma region Enemies
		case eID::ZOMMER:
		{
			Zommer* zommer = static_cast<Zommer*>(i->object);
			if (zommer->getCold())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:
					this->samus->setVelocityX(0);
					//not allow move left
					this->samus->setCanMoveRight(false);
					break;
				case CollideDirection::RIGHT:
					this->samus->setVelocityX(0);
					//not allow move right
					this->samus->setCanMoveLeft(false);
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);
					break;
				}
			}
			else if (zommer->getExplose())
			{
				zommer->setCanDraw(false);
			}
			else
			{
				SamusStateManager::getInstance()->setOldStatus(eStatus::ROLLING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}
		case eID::WAVER:
		{
			Waver* waver = static_cast<Waver*>(i->object);
			if (waver->getCold())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:
					this->samus->setVelocityX(0);
					//not allow move left
					this->samus->setCanMoveRight(false);
					break;
				case CollideDirection::RIGHT:
					this->samus->setVelocityX(0);
					//not allow move right
					this->samus->setCanMoveLeft(false);
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);
					break;
				}
			}
			else if (waver->getExplose())
			{
				waver->setCanDraw(false);
			}
			else
			{
				SamusStateManager::getInstance()->setOldStatus(eStatus::ROLLING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}

		case eID::SKREE:
		{
			Skree* skree = static_cast<Skree*>(i->object);
			if (!skree->getHandle())
			{
				return;
			}
			if (skree->getCold())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:
					this->samus->setVelocityX(0);
					//not allow move left
					this->samus->setCanMoveRight(false);
					break;
				case CollideDirection::RIGHT:
					this->samus->setVelocityX(0);
					//not allow move right
					this->samus->setCanMoveLeft(false);
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);
					break;
				}
			}
			else if (skree->getExplose())
			{
				skree->setCanDraw(false);
				skree->setActivity(false);
			}
			else
			{
				SamusStateManager::getInstance()->setOldStatus(eStatus::ROLLING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}
		case eID::RIO:
		case eID::RIPPER:
		
		case eID::ZEB:
		case eID::BOSSKRAID:
		{
			if (i->object->isActivitied())
			{
				switch (i->direction)
				{
				case CollideDirection::LEFT:
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::RIGHT:
					this->samus->setVelocityX(0);
					break;
				case CollideDirection::TOP:
					this->samus->setVelocityY(0);
					break;
				}
				SamusStateManager::getInstance()->setOldStatus(eStatus::ROLLING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}

			break;
		}
#pragma endregion

		}

	}
	
}
void SamusStateRolling::update(float dt)
{
	this->animation->update(dt);

	if (this->samus->canMoveLeft() || this->samus->canMoveRight())
	{
		this->samus->updateHorizontal(dt);
	}
	this->samus->updateVertical(dt);

	setBoundCollision();

	timer += dt;
	if (timer < 0.03f)
	{
		if (flagUpDown)
		{
			this->offsetY = this->samus->getPosition().y;
			this->samus->setPositionY(this->samus->getPosition().y - ACCELERATE_Y*dt);
		}
			
	}
	else
	{
		flagUpDown = !flagUpDown;
		timer = 0;
	}

	if (move_to_fall && !this->samus->isInStatus(eStatus::STANDING)&& !this->samus->isInStatus(eStatus::INJURING)
		&&!samus->getBoomExplose())
	{
		this->samus->setStatus(eStatus::FALLING_ROLLING);
	}
	if(samus->getBoomExplose())
	{
		this->samus->setStatus(eStatus::BOOM);
		SamusStateManager::getInstance()->setOldState(this);
		SamusStateManager::getInstance()->setOldStatus(eStatus::ROLLING);
	}
	if (!this->samus->isInStatus(eStatus::ROLLING))
	{
		switch (this->samus->getStatus())
		{
		case eStatus::STANDING:
			if(canStanding)
			{
				this->samus->setPositionY(this->samus->getPosition().y + OFFSET_STAND);
				SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			}
			break;
		case eStatus::FALLING_ROLLING:
			SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			break;
		case eStatus::INJURING:
			SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			break;
		case eStatus::BOOM:
			SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			break;
		default:

			break;
		}
	
		return;
	}
	this->samus->setCanMoveLeft(true);
	this->samus->setCanMoveRight(true);

	move_to_fall = true;
	canStanding = false;
}


void SamusStateRolling::onStart()
{
	this->animation->start();
}

void SamusStateRolling::onExit()
{
	this->animation->stop();
}
