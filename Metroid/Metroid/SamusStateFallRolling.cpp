#include "SamusStateFallRolling.h"
#include "SamusStateManager.h"
#include "Camera.h"
#include "GameLog.h"

#include "Skree.h"
#include "Zommer.h"
#include "Waver.h"
#include "Zeb.h"
#include "Rio.h"
#include "GateBlue.h"
#include "GateRed.h"
#include "Brick.h"
SamusStateFallRolling::SamusStateFallRolling()
{
}

SamusStateFallRolling::SamusStateFallRolling(Samus * samus, Input * input) : BaseState(samus, input)
{

}


SamusStateFallRolling::~SamusStateFallRolling()
{

}

void SamusStateFallRolling::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y);
	rect.left = position.x - WIDTH_COLLISION*0.5f;
	rect.right = position.x + WIDTH_COLLISION*0.5f;
	rect.top = position.y + ROLL_HEIGHT*0.5f;
	rect.bottom = position.y - ROLL_HEIGHT*0.5f;
	samus->setBoundCollision(rect);
}

void SamusStateFallRolling::init()
{
	this->animation = samus->getRollingAnim();

	this->samus->setVelocityY(SAMUS_V0_FALL_Y);
}

void SamusStateFallRolling::handleInput(float dt)
{
	if (!Camera::getInstance()->moveWhenSamusOnPort()) 
	{
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
				if (this->samus->isFaling())
					this->samus->setVelocityX(-SAMUS_VELOCITY_JUMP_X);
				else
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
				if (this->samus->isFaling())
					this->samus->setVelocityX(SAMUS_VELOCITY_JUMP_X);
				else
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

		if (this->samus->getVelocity().y > -SAMUS_MAX_SPEED_Y)
		{
			this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);
		}
	}
}

void SamusStateFallRolling::onCollision(float dt)
{
	if (this->samus->isInStatus(eStatus::STANDING))
	{
		MetroidRect bound;
		bound.left = this->samus->getPosition().x - WIDTH_COLLISION*0.5f;
		bound.right = this->samus->getPosition().x + WIDTH_COLLISION*0.5f;
		bound.bottom = this->samus->getBoundCollision().bottom;
		bound.top = bound.bottom + 30;

		for (map<int,BaseObject*>::iterator i = this->samus->getListCanCollide()->begin(); i != this->samus->getListCanCollide()->end(); ++i)
		{
			if (Collision::getInstance()->isCollide((*i).second->getBoundCollision(), bound))
			{
				
				if ((*i).second->getId() == eID::BRICK)
				{
					Brick* brick = static_cast<Brick*>((*i).second);
					if (brick->getVisible())
					{
						this->samus->setStatus(eStatus::FALLING_ROLLING);
					}
				}
				else
				{
					this->samus->setStatus(eStatus::FALLING_ROLLING);
				}
			}
		}
	}
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
		case eID::WALL:
		case eID::BRICK:
		case eID::FIRE:
		case eID::ELEVATOR:
		case eID::ALIENBIG:
		case eID::ALIENSMALL:
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
				//remain_time = 1.0f - i->entryTime;
				this->samus->setVelocityY(0);
				this->samus->setPositionY(i->positionCollision + OFFSET_ROLLING);
				this->samus->setStatus(eStatus::ROLLING);
				break;
			}
			break;
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
				SamusStateManager::getInstance()->setOldStatus(eStatus::FALLING_ROLLING);
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
				SamusStateManager::getInstance()->setOldStatus(eStatus::FALLING_ROLLING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}
		case eID::SKREE:
		case eID::RIO:
		case eID::RIPPER:
		case eID::ZEB:
		case eID::BOSSKRAID:
			switch (i->direction)
			{
			case CollideDirection::LEFT:
				this->samus->setVelocityX(0);
				//not allow move left
				//this->samus->setCanMoveRight(false);
				break;
			case CollideDirection::RIGHT:
				this->samus->setVelocityX(0);
				//not allow move right
				//this->samus->setCanMoveLeft(false);
				break;
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);

				this->samus->setPositionY(i->positionCollision + OFFSET_ROLLING);

				break;
			}
			//this->samus->setVelocity(VECTOR2(0, 0));
			SamusStateManager::getInstance()->setOldStatus(eStatus::FALLING_ROLLING);
			SamusStateManager::getInstance()->setOldState(this);
			this->samus->setStatus(eStatus::INJURING);
			break;
		default:
			break;
		}

	}

}
void SamusStateFallRolling::update(float dt)
{
	this->animation->update(dt);
	if (this->samus->canMoveLeft() || this->samus->canMoveRight())
	{
		this->samus->updateHorizontal(dt);
	}
	this->samus->updateVertical(dt);

	setBoundCollision();

	if (this->samus->getStatus() != eStatus::FALLING_ROLLING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::STANDING:
			//this->samus->setPositionY(this->samus->getPosition().y + OFFSET_STAND);
			this->samus->setFall(true);
			break;
		default:
			break;
		}
		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());

	}

	this->samus->setCanMoveLeft(true);
	this->samus->setCanMoveRight(true);

	//this->samus->setVelocityX(0);
	

}


void SamusStateFallRolling::onStart()
{
	this->animation->start();
}

void SamusStateFallRolling::onExit()
{
	this->animation->stop();
}
