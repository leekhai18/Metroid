#include "SamusStateJumping.h"
#include "SamusStateManager.h"
#include "GameLog.h"
#include "Camera.h"
#include "GateBlue.h"
#include "GateRed.h"
#include "MaruMari.h"
#include "EnergyTank.h"
#include "Bomb.h"
#include "LongBeam.h"
#include "MissileRocket.h"
#include "IceBeam.h"
#include "Varia.h"
#include "Skree.h"
#include "Zommer.h"
#include "Waver.h"
#include "Zeb.h"
#include "Rio.h"
#include "Ripper.h"
#include "Port.h"
#include "ScenceManager.h"
#include "Sound.h"
#include "Brick.h"
SamusStateJumping::SamusStateJumping()
{
}

SamusStateJumping::SamusStateJumping(Samus * samus, Input * input) : BaseState(samus, input)
{
}

SamusStateJumping::~SamusStateJumping()
{
}

void SamusStateJumping::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y );
	rect.left = position.x - JUMP_WIDTH*0.5f;
	rect.right = position.x + JUMP_WIDTH*0.5f;
	rect.top = position.y + JUMP_HEIGHT*0.5f;
	rect.bottom = position.y - JUMP_HEIGHT*0.5f;
	samus->setBoundCollision(rect);
}
void SamusStateJumping::init()
{
	Sound::getInstance()->play(SOUND_JUMP, false);
	time = 0;
	if (this->samus->isFaling() == false)
	{
		this->samus->setVelocityY(SAMUS_MAX_SPEED_Y);
	}
		
	
	this->samus->setDataSuiteSkin(
		IndexManager::getInstance()->samusYellowJumpRight,
		IndexManager::getInstance()->samusYellowIceJumpRight,
		IndexManager::getInstance()->samusPinkJumpRight,
		IndexManager::getInstance()->samusPinkIceJumpRight);

	setBoundCollision();
}

void SamusStateJumping::handleInput(float dt)
{
	this->samus->setVelocityX(0);
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

			// Handle direction	
			if (this->samus->isInDirection(eDirection::right))
			{
				this->samus->setFlipX(true);
				this->samus->setDirection(eDirection::left);
			}

			this->samus->setVelocityX(-SAMUS_VELOCITY_JUMP_X);
		}

		/*if ((input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT)))
		{
			this->samus->setVelocityX(0);
		}*/
#pragma endregion

#pragma region Vertical
		// Handle vertical
		if (this->samus->isFaling() == false) // is going up
		{
			//samus.velocity.y= V + at
			this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);

			jumpDistance += this->samus->getVelocity().y*dt;
			time += dt;

			//if acrobat and time enough to change acrobat state
			if (time >= TIME_TO_ACROBAT && samus->isAcrobat())
			{
				samus->setStatus(eStatus::ACROBAT);
			}

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
		else // Falling
		{
			if (this->samus->getVelocity().y > -SAMUS_MAX_SPEED_Y)
				this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);
		}
#pragma endregion


#pragma region Shooting

		if (this->animation == nullptr)
		{
			if (input->isKeyDown(VK_Z) && input->isKeyUp(VK_UP))
			{
				// Set Data for sprite
				this->samus->setDataSuiteSkin(
					IndexManager::getInstance()->samusYellowHittingAndJumpRight,
					IndexManager::getInstance()->samusYellowIceHittingAndJumpRight,
					IndexManager::getInstance()->samusPinkHittingAndJumpRight,
					IndexManager::getInstance()->samusPinkIceHittingAndJumpRight);


				this->isUp = false;
				if (this->samus->timerShoot > TIME_SHOOTING)
				{
					this->fire();
					this->samus->timerShoot = 0;
				}
			}

			if (input->isKeyDown(VK_Z) && input->isKeyDown(VK_UP))
			{
				// Set Data for sprite
				this->samus->setDataSuiteSkin(
					IndexManager::getInstance()->samusYellowHittingJumpUp,
					IndexManager::getInstance()->samusYellowIceHittingJumpUp,
					IndexManager::getInstance()->samusPinkHittingJumpUp,
					IndexManager::getInstance()->samusPinkIceHittingJumpUp);


				this->isUp = true;
				if (this->samus->timerShoot > TIME_SHOOTING)
				{
					this->fire();
					this->samus->timerShoot = 0;
				}
			}
			if (input->isKeyDown(VK_DOWN) && this->samus->isHaveMariMaru())
			{
				this->samus->setVelocityX(0);
				this->samus->setStatus(eStatus::ROLLING);
				jumpDistance = 0;
				return;
			}
			if (input->isKeyDown(VK_UP) && input->isKeyUp(VK_Z))
			{
				// Set Data for sprite
				this->samus->setDataSuiteSkin(
					IndexManager::getInstance()->samusYellowJumpUp,
					IndexManager::getInstance()->samusYellowIceJumpUp,
					IndexManager::getInstance()->samusPinkJumpUp,
					IndexManager::getInstance()->samusPinkIceJumpUp);

			}

			if (input->isKeyUp(VK_Z) && input->isKeyUp(VK_UP) && this->samus->isInStatus(eStatus::JUMPING))
			{
				// Set Data for sprite
				this->samus->setDataSuiteSkin(
					IndexManager::getInstance()->samusYellowJumpRight,
					IndexManager::getInstance()->samusYellowIceJumpRight,
					IndexManager::getInstance()->samusPinkJumpRight,
					IndexManager::getInstance()->samusPinkIceJumpRight);
			}
		}
#pragma endregion
	}
}

void SamusStateJumping::onCollision(float dt)
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
		
		case eID::ALIENBIG:
		case eID::ALIENSMALL:
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
				//if(this->samus->getStatus()!)
				this->samus->setStatus(eStatus::STANDING);
				break;
			case CollideDirection::BOTTOM:
				jumpDistance = 0;
				this->samus->setFall(true);
				this->samus->setVelocityY(0);
				addY = i->positionCollision;
				this->samus->setVelocityY(0);

				this->samus->setPositionY(addY - OFFSET_JUMP);
				break;
			}
			break;

#pragma endregion

		case eID::ELEVATOR:
		{
			if (samus->getCanMoveElevator())
			{
				return;
			}

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
				this->samus->setVelocityY(0);

				this->samus->setPositionY(addY - OFFSET_JUMP);
				break;
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
				case CollideDirection::TOP:
					jumpDistance = 0;
					this->samus->setFall(true);
					this->samus->setVelocityY(0);
					addY = i->positionCollision;
					this->samus->setVelocityY(0);

					this->samus->setPositionY(addY - OFFSET_JUMP);
					break;
				}
			}

			break;
		}

		case eID::PORT:
		{
			Port* port = static_cast<Port*>(i->object);
			if(port->Win()==true)
			{
				this->samus->setVelocity(VECTOR2ZERO);
				ScenceManager::getInstance()->goToScence(ScenceType::END);
				return;
			}
			switch (i->direction)
			{
				
			case LEFT:
				
				Camera::getInstance()->setVelocity(VECTOR2(SAMUS_VERLOCITY_X, 0));
				Camera::getInstance()->setOnPort(true);
				this->samus->setIsCollidingPort(true);
				break;
			case RIGHT:
				Camera::getInstance()->setVelocity(VECTOR2(-SAMUS_VERLOCITY_X, 0));
				Camera::getInstance()->setOnPort(true);
				this->samus->setIsCollidingPort(true);
				break;
			default:
				break;
			}

			break;
		}

#pragma endregion

#pragma region enemies
		case eID::FIRE_BUBLE:
		{
			switch (i->direction)
			{
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);
				break;
			}
			//Buble* zommer = static_cast<Buble*>(i->object);
			SamusStateManager::getInstance()->setOldStatus(eStatus::JUMPING);
			this->samus->setStatus(eStatus::INJURING);
			SamusStateManager::getInstance()->setOldState(this);
			break;
		}
		case eID::MACHINE_CANON:
		{
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
				addY = i->positionCollision;
				this->samus->setVelocityY(0);
				this->samus->setPositionY(addY - OFFSET_JUMP);
				break;
			}
			break;
		}
		case eID::ZOMMER:
		{
			Zommer* zommer = static_cast<Zommer*>(i->object);
			if (zommer->getCold()&& zommer->getHandle())
			{
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
					addY = i->positionCollision;
					this->samus->setVelocityY(0);
					this->samus->setPositionY(addY - OFFSET_JUMP);
					break;
				}
			}
			else if (zommer->getExplose())
			{
				zommer->setCanDraw(false);
				zommer->setActivity(false);
			}
			else
			{
				if (!zommer->getHandle())
				{
					return;
				}
				SamusStateManager::getInstance()->setOldStatus(eStatus::JUMPING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}
		case eID::WAVER:
		{
			Waver* waver = static_cast<Waver*>(i->object);
			if (waver->getCold()&& waver->getHandle())
			{
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
					/*if(this->samus->isFaling())
					{
						break;
					}*/
					jumpDistance = 0;
					this->samus->setFall(true);
					addY = i->positionCollision;
					this->samus->setVelocityY(0);

					this->samus->setPositionY(addY - OFFSET_JUMP);
					break;
				}
				break;
			}
			else if (waver->getExplose())
			{
				waver->setCanDraw(false);
				waver->setActivity(false);
			}
			else
			{
				if (!waver->getHandle())
				{
					return;
				}
				SamusStateManager::getInstance()->setOldStatus(eStatus::JUMPING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}
		case eID::SKREE:
		{
			Skree* skree = static_cast<Skree*>(i->object);
			
			if (skree->getCold()&& skree->getHandle())
			{
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
					addY = i->positionCollision;
					this->samus->setVelocityY(0);

					this->samus->setPositionY(addY - OFFSET_JUMP);
					break;
				}
				break;
			}
			else if (skree->getExplose())
			{
				skree->setCanDraw(false);
				skree->setActivity(false);
			}
			else
			{
				if (!skree->getHandle())
				{
					return;
				}
				SamusStateManager::getInstance()->setOldStatus(eStatus::JUMPING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}
		case eID::RIO:
		{
			Rio* rio = static_cast<Rio*>(i->object);

			if (rio->getCold()&& rio->getHandle())
			{
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
					/*if(this->samus->isFaling())
					{
					break;
					}*/
					jumpDistance = 0;
					this->samus->setFall(true);
					addY = i->positionCollision;
					this->samus->setVelocityY(0);

					this->samus->setPositionY(addY - OFFSET_JUMP);
					break;
				}
				break;
			}
			else if (rio->getExplose())
			{
				rio->setCanDraw(false);
				rio->setActivity(false);
			}
			else
			{
				if (!rio->getHandle())
				{
					return;
				}
				SamusStateManager::getInstance()->setOldStatus(eStatus::JUMPING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}
		case eID::RIPPER:
		{
			Ripper* ripper = static_cast<Ripper*>(i->object);
			if (ripper->getCold())
			{
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
					/*if(this->samus->isFaling())
					{
					break;
					}*/
					jumpDistance = 0;
					this->samus->setFall(true);
					addY = i->positionCollision;
					this->samus->setVelocityY(0);

					this->samus->setPositionY(addY - OFFSET_JUMP);
					break;
				}
				break;
			}
			else
			{
				SamusStateManager::getInstance()->setOldStatus(eStatus::JUMPING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}
		case eID::ZEB:
		{
			Zeb* zeb = static_cast<Zeb*>(i->object);

			if (zeb->getCold()&& zeb->getHandle())
			{
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
					addY = i->positionCollision;
					this->samus->setVelocityY(0);

					this->samus->setPositionY(addY - OFFSET_JUMP);
					break;
				}
				break;
			}
			else if (zeb->getExplose())
			{
				zeb->setCanDraw(false);
				zeb->setActivity(false);
			}
			else
			{
				if (!zeb->getHandle())
				{
					return;
				}
				SamusStateManager::getInstance()->setOldStatus(eStatus::JUMPING);
				this->samus->setStatus(eStatus::INJURING);
				SamusStateManager::getInstance()->setOldState(this);
			}
			break;
		}
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
			SamusStateManager::getInstance()->setOldStatus(eStatus::JUMPING);
			SamusStateManager::getInstance()->setOldState(this);
			this->samus->setStatus(eStatus::INJURING);
			break;
		}
		case eID::MOTHERBRAIN:
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
			}

			SamusStateManager::getInstance()->setOldStatus(eStatus::JUMPING);
			this->samus->setStatus(eStatus::INJURING);
			SamusStateManager::getInstance()->setOldState(this);

			break;
		}

#pragma endregion

#pragma region items
		case eID::MARUMARI:
		{
			this->samus->setMariMaru(true);
			MaruMari* mm = static_cast<MaruMari*>(i->object);
			mm->setActivity(false);

			break;
		}

		case eID::ENERGYTANK:
		{
			this->samus->setNumLive(this->samus->getNumLive() + 1);
			EnergyTank* energy = static_cast<EnergyTank*>(i->object);
			energy->setActivity(false);

			break;
		}

		case eID::BOMB:
		{
			this->samus->setMariMaru(true); // just test

			this->samus->setBomb(true);
			Bomb* bom = static_cast<Bomb*>(i->object);
			bom->setActivity(false);

			break;
		}

		case eID::LONGBEAM:
		{
			this->samus->setLongBeam(true);
			LongBeam* lb = static_cast<LongBeam*>(i->object);
			lb->setActivity(false);

			BulletPool::getInstance()->setDistanceShoot(DISTANCE_SHOOT*2);

			break;
		}

		case eID::MISSILEROCKET:
		{
			this->samus->setNumRocket(this->samus->getNumRocket() + 5);
			MissileRocket* mrocket = static_cast<MissileRocket*>(i->object);
			mrocket->setActivity(false);

			break;
		}

		case eID::ICEBEAM:
		{
			IceBeam* ib = static_cast<IceBeam*>(i->object);
			ib->setActivity(false);

			if (this->samus->getCurrentSkin() == eSkin::YELLOW)
				this->samus->setNewSkin(eSkin::YELLOWICE);
			if (this->samus->getCurrentSkin() == eSkin::PINK)
				this->samus->setNewSkin(eSkin::PINKICE);

			BulletPool::getInstance()->setIceBullet();

			break;
		}

		case eID::VARIA:
		{
			Varia* va = static_cast<Varia*>(i->object);
			va->setActivity(false);

			if (this->samus->getCurrentSkin() == eSkin::YELLOW)
				this->samus->setNewSkin(eSkin::PINK);
			if (this->samus->getCurrentSkin() == eSkin::YELLOWICE)
				this->samus->setNewSkin(eSkin::PINKICE);

			break;
		}

#pragma endregion
		default:
			break;
		}

	}
	if (this->samus->isInStatus(eStatus::STANDING))
	{
		VECTOR2 position = VECTOR2(this->samus->getPosition().x, positionCollide + OFFSET_STAND - OFFSET_COLLISION_Y + 1);
		MetroidRect bound;
		bound.left = position.x - WIDTH_COLLISION*0.5f;
		bound.right = position.x + WIDTH_COLLISION*0.5f;
		bound.top = position.y + HEIGHT_COLLISION*0.5f;
		bound.bottom = position.y - HEIGHT_COLLISION*0.5f;


		for (auto i = this->samus->getListWallCanCollide()->begin(); i != this->samus->getListWallCanCollide()->end(); ++i)
		{
			if((*i).second->isActivitied())
			{
				if (Collision::getInstance()->isCollide((*i).second->getBoundCollision(), bound))
				{
					
						this->samus->setStatus(eStatus::JUMPING);
						break;
				}
			}
			
		}
	}
}

void SamusStateJumping::update(float dt)
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
	if (this->samus->getStatus() != eStatus::JUMPING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::STANDING:

			this->samus->setPositionY(positionCollide +  OFFSET_STAND);

			break;
		case eStatus::ACROBAT:
			break;
		case eStatus::INJURING:
			/*if(this->samus->isFaling())
			{
				this->samus->setPositionY(positionCollide - OFFSET_STAND);
			}
			else
			{
				this->samus->setPositionY(positionCollide + OFFSET_STAND);
			}*/
			
			break;
			case  eStatus::ROLLING:
				this->samus->setCanJump(true);
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



void SamusStateJumping::onStart()
{
	if (this->animation != nullptr)
		this->animation->start();
}

void SamusStateJumping::onExit()
{
	if (this->animation != nullptr)
	{
		this->animation->stop();
		this->animation = nullptr;
	}
}

void SamusStateJumping::fire()
{
	VECTOR2 stP;
	Bullet* bullet = BulletPool::getInstance()->getBullet();
	Sound::getInstance()->play(SOUND_BULLET, false);

	if (isUp)
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection(), this->samus->getPosition().y + this->samus->getSprite()->getHeight()*0.5f);
		bullet->setVelocity(VECTOR2(0, VELOCITY_BULLET));
	}
	else
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection()*this->samus->getSprite()->getWidth()*0.2f, this->samus->getPosition().y + 2);
		bullet->setVelocity(VECTOR2((float)VELOCITY_BULLET*this->samus->getDirection(), 0));
	}

	bullet->init(stP);
}


