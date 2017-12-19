#include "SamusStateAcrobat.h"
#include "SamusStateManager.h"
#include "GameLog.h"
#include "MaruMari.h"
#include "Camera.h"
#include "GateBlue.h"
#include "GateRed.h"

SamusStateAcrobat::SamusStateAcrobat()
{
}

SamusStateAcrobat::SamusStateAcrobat(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateAcrobat::~SamusStateAcrobat()
{
}

void SamusStateAcrobat::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y );
	rect.left = position.x - WIDTH_COLLISION*0.5f;
	rect.right = position.x + WIDTH_COLLISION*0.5f;
	rect.top = position.y + JUMP_HEIGHT*0.5f;
	rect.bottom = position.y - JUMP_HEIGHT*0.5f;
	samus->setBoundCollision(rect);
}
void SamusStateAcrobat::init()
{
	this->animation = samus->getJumpingAnim();
	time = 0;
	this->position_to_jump = this->samus->getPosition().y;
	setBoundCollision();
}

void SamusStateAcrobat::handleInput(float dt)
{
	if (!Camera::getInstance()->moveWhenSamusOnPort())
	{
#pragma region Horizontal
		if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
		{
			// Handle direction
			if (this->samus->isInDirection(eDirection::left))
			{
				this->samus->setFlipX(false);
				this->samus->setDirection(eDirection::right);
			}
			this->samus->setVelocityX(SAMUS_VELOCITY_JUMP_X);
		}

		if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
		{
			if (this->samus->isInDirection(eDirection::right))
			{
				this->samus->setFlipX(true);
				this->samus->setDirection(eDirection::left);
			}

			this->samus->setVelocityX(-SAMUS_VELOCITY_JUMP_X);
		}

		if ((input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT)))
		{
			this->samus->setVelocityX(0);
		}
#pragma endregion

#pragma region Vertical
		// Handle vertical
		if (this->samus->isFaling() == false) // is going up
		{
			//samus.velocity.y= V + at
			this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);

			jumpDistance += this->samus->getVelocity().y*dt;
			time += dt;

			if (jumpDistance < MAX_JUMP)
			{
				this->samus->setFall(false);
			}
			else
			{
				jumpDistance = 0;
				this->samus->setFall(true);
				this->samus->setVelocityY(SAMUS_V0_FALL_Y);
			}

			//if we release jump button, samus will jump to MIN_JUMP
			if (input->isKeyUp(VK_X))
			{
				this->samus->setFall(true);
				this->samus->setVelocityY(SAMUS_V0_FALL_Y);
			}
		}
		else //Falling
		{
			if (this->samus->getVelocity().y > -SAMUS_MAX_SPEED_Y)
				this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);

			if (this->samus->getPosition().y <= position_to_jump)
				this->samus->setStatus(eStatus::JUMPING);
		}
#pragma endregion
	}
}

#pragma region Collision
void SamusStateAcrobat::onCollision(float dt)
{
	float addX = 0, addY = 0;

	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
#pragma region Wall
		case eID::WALL:
		case eID::BRICK:
		case eID::FIRE:
		case eID::ELEVATOR:
			switch (i->direction)
			{
			case CollideDirection::LEFT:
				this->samus->setCanMoveRight(false);
				this->samus->setVelocityX(0);
				break;

			case CollideDirection::RIGHT:
				this->samus->setCanMoveLeft(false);
				this->samus->setVelocityX(0);
				break;

			case CollideDirection::TOP:
				jumpDistance = 0;
				//set jump = false, when user release jump button set to true
				this->samus->setCanJump(false);
				//set fall to false
				this->samus->setFall(false);
				//reset velocity
				this->samus->setVelocityY(0);
				positionCollide = i->positionCollision;
				this->samus->setStatus(eStatus::STANDING);
				break;

			case CollideDirection::BOTTOM:
				jumpDistance = 0;
				this->samus->setFall(true);
				this->samus->setVelocityY(0);
				addY = i->positionCollision;
				this->samus->setPositionY(addY - OFFSET_JUMP);
				break;
			}

			break;
#pragma endregion

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
			//another object
		case eID::ZOMMER:
		case eID::SKREE:
		case eID::RIO: 
		case eID::RIPPER:
		case eID::WAVER:
		case eID::ZEB:
		case eID::BOSSKRAID:
		{
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
				break;
			case CollideDirection::BOTTOM:
				this->samus->setVelocityY(0);
				break;
			}
			//this->samus->setVelocity(VECTOR2(0, 0));
			SamusStateManager::getInstance()->setOldStatus(eStatus::ACROBAT);
			SamusStateManager::getInstance()->setOldState(this);
			this->samus->setStatus(eStatus::INJURING);

			break;
		}
#pragma endregion

#pragma region Items
		case eID::MARUMARI:
		{
			this->samus->setMariMaru(true);

			MaruMari* mm = static_cast<MaruMari*>(i->object);
			mm->setActivity(false);

			break;
		}
#pragma endregion

		default:
			break;
		}
	}
}
#pragma endregion

void SamusStateAcrobat::update(float dt)
{
	if (this->animation != nullptr)
	{
		this->animation->update(dt);
	}
	//update position.x
	if (this->samus->canMoveLeft() || this->samus->canMoveRight())
	{
		this->samus->updateHorizontal(dt);
	}
	//update position.y
	this->samus->updateVertical(dt);
	setBoundCollision();

#pragma region ChangeState
	if (this->samus->getStatus() != eStatus::ACROBAT)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::JUMPING:

			break;
		case eStatus::STANDING:
			this->samus->setPositionY(positionCollide + OFFSET_STAND);
			//this->samus->setJump(false);
			break;
		default:
			break;
		}
		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}
#pragma endregion

	this->samus->setCanMoveLeft(true);
	this->samus->setCanMoveRight(true);

}



void SamusStateAcrobat::onStart()
{
	this->animation->start();
}

void SamusStateAcrobat::onExit()
{
	this->animation->stop();
	this->animation = nullptr;	
}



