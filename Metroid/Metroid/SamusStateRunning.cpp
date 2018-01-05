#include "SamusStateRunning.h"
#include "SamusStateManager.h"
#include "GameLog.h"
#include "Camera.h"
#include "GateBlue.h"
#include "Camera.h"

#include "Skree.h"
#include "Zommer.h"
#include "Waver.h"
#include "Zeb.h"
#include "Rio.h"
#include "Ripper.h"
#include "MotherBrain.h"
#include "Sound.h"
#include "ScenceManager.h"
SamusStateRunning::SamusStateRunning()
{
}

SamusStateRunning::SamusStateRunning(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateRunning::~SamusStateRunning()
{

}
void SamusStateRunning::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y - OFFSET_COLLISION_Y + 1);
	rect.left = position.x - WIDTH_COLLISION*0.5f;
	rect.right = position.x + WIDTH_COLLISION*0.5f;
	rect.top = position.y + HEIGHT_COLLISION*0.5f;
	rect.bottom = position.y - HEIGHT_COLLISION*0.5f;
	samus->setBoundCollision(rect);
}



void SamusStateRunning::init()
{
	runningNormal = samus->getRunningNormalAnim();
	runningUp = samus->getRunningUpAnim();
	runningHittingRight = samus->getRunningHittingRightAnim();

	//this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);
	this->animation = runningNormal;
	this->animation->setValueOfCurrentFrame(0);
	setBoundCollision();
}

void SamusStateRunning::handleInput(float dt)
{
	this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);

	if (!Camera::getInstance()->moveWhenSamusOnPort()) 
	{
#pragma region Horizontal
		if ((input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT))
			|| (input->isKeyDown(VK_RIGHT) && this->samus->isInDirection(eDirection::left))
			|| (input->isKeyDown(VK_LEFT) && this->samus->isInDirection(eDirection::right)))
		{
			//reset velocity
			this->samus->setVelocityX(0);
			this->samus->setStatus(eStatus::STANDING);
			return;
		}
		if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
		{
			this->samus->setVelocityX(-SAMUS_VERLOCITY_X);
		}

		if (input->isKeyUp(VK_LEFT) && input->isKeyDown(VK_RIGHT))
		{
			this->samus->setVelocityX(SAMUS_VERLOCITY_X);
		}


#pragma endregion
		if (input->isKeyUp(VK_X) == true)
		{
			this->samus->setCanJump(true);
		}
		//When we press jump key, change state to jump
		if (input->isKeyDown(VK_X) && this->samus->canJump() == true)
		{
			//reset velocity
			this->samus->setFall(false);
			this->samus->setAcrobat(true);
			this->samus->setStatus(eStatus::JUMPING);
			return;
		}

		//when we press up button hand of samus will raise to sky
		if (input->isKeyDown(VK_UP))
		{
			//stop current animation
			this->animation->stop();
			//change animation to hand raise to sky
			this->animation = runningUp;
			//start new animation
			this->animation->start();
		}
		//when we press shot button and not press up button, samus will shot horizontal 
		if (input->isKeyDown(VK_Z) && input->isKeyUp(VK_UP))
		{
			//stop current animation
			this->animation->stop();
			//change animation to shot horizontal
			this->animation = runningHittingRight;
			//start new animation
			this->animation->start();

			//we must press shot button enoung long to samus shot
			this->isUp = false;
			if (this->samus->timerShoot > TIME_SHOOTING)
			{
				this->fire();
				this->samus->timerShoot = 0;
			}
		}

		if (input->isKeyDown(VK_UP) && input->isKeyDown(VK_Z))
		{
			this->animation->stop();

			this->samus->setDataSuiteSkin(
				IndexManager::getInstance()->samusYellowHittingUp,
				IndexManager::getInstance()->samusYellowIceHittingUp,
				IndexManager::getInstance()->samusPinkHittingUp,
				IndexManager::getInstance()->samusPinkIceHittingUp);

			this->animation = runningUp;
			this->animation->start();

			this->isUp = true;
			if (this->samus->timerShoot > TIME_SHOOTING)
			{
				this->fire();
				this->samus->timerShoot = 0;
			}
		}

		if (input->isKeyUp(VK_UP) && input->isKeyUp(VK_Z) && (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT)))
		{
			this->animation->stop();

			this->animation = runningNormal;
			this->animation->start();
		}
	}

}
void SamusStateRunning::onCollision(float dt)
{
	MetroidRect bound;
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
#pragma region Wall
		case eID::WALL:
		case eID::BRICK:
		case eID::ALIENBIG:
		case eID::ALIENSMALL:
			switch (i->direction)
			{
			case CollideDirection::LEFT:
				if (this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top)
				{
					this->samus->setVelocityX(0);
					this->samus->setCanMoveRight(false);
					this->samus->setStatus(eStatus::STANDING);
				}
				break;
			case CollideDirection::RIGHT:

				if (this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top)
				{
					this->samus->setVelocityX(0);
					this->samus->setCanMoveLeft(false);
					this->samus->setStatus(eStatus::STANDING);
				}
				break;
			case CollideDirection::TOP:
				this->samus->setFall(false);
				//this->samus->setPositionY()
				this->samus->setPositionY(i->positionCollision + OFFSET_STAND);
				this->samus->setVelocityY(0);
				break;
			}
			break;

#pragma endregion
		case eID::FIRE:
		{
			switch (i->direction)
			{
			case CollideDirection::TOP:
			{
				this->samus->setFall(false);
				this->samus->setVelocityY(0);
				this->samus->setHealth(this->samus->getHealth() - 8);
				break;
			}
			break;
			}
		}
#pragma region Elevator
		case eID::ELEVATOR:
			if (samus->getCanMoveElevator())
			{
				return;
			}
			switch (i->direction)
			{
			case CollideDirection::LEFT:

				if (this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top)
				{
					this->samus->setVelocityX(0);
					this->samus->setCanMoveRight(false);
					this->samus->setStatus(eStatus::STANDING);
				}
				break;
			case CollideDirection::RIGHT:

				if (this->samus->getBoundCollision().bottom < i->object->getBoundCollision().top)
				{
					this->samus->setVelocityX(0);
					this->samus->setCanMoveLeft(false);
					this->samus->setStatus(eStatus::STANDING);
				}
				break;
			case CollideDirection::TOP:
				this->samus->setFall(false);
				this->samus->setVelocityY(0);
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
				case CollideDirection::TOP:
					this->samus->setFall(false);
					this->samus->setVelocityY(0);
					break;
				}
			}

			break;
		}

		case eID::PORT:
		{
			Port* port = static_cast<Port*>(i->object);
			if (port->Win() == true)
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

		case eID::FIRE_BUBLE:
		{
			switch (i->direction)
			{
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);
				break;
			}
			//Buble* zommer = static_cast<Buble*>(i->object);
			SamusStateManager::getInstance()->setOldStatus(eStatus::RUNNING);
			this->samus->setStatus(eStatus::INJURING);
			SamusStateManager::getInstance()->setOldState(this);
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
					this->samus->setFall(false);
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
				SamusStateManager::getInstance()->setOldStatus(eStatus::RUNNING);
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
					this->samus->setFall(false);
					this->samus->setVelocityY(0);
					break;
				}
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
				SamusStateManager::getInstance()->setOldStatus(eStatus::RUNNING);
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
					this->samus->setFall(false);
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
				if (!skree->getHandle())
				{
					return;
				}
				SamusStateManager::getInstance()->setOldStatus(eStatus::RUNNING);
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
					this->samus->setFall(false);
					this->samus->setVelocityY(0);
					break;
				}
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
				SamusStateManager::getInstance()->setOldStatus(eStatus::RUNNING);
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
					this->samus->setFall(false);
					this->samus->setVelocityY(0);
					break;
				}
			}
			else
			{

				SamusStateManager::getInstance()->setOldStatus(eStatus::RUNNING);
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
					this->samus->setFall(false);
					this->samus->setVelocityY(0);
					break;
				}
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
				SamusStateManager::getInstance()->setOldStatus(eStatus::RUNNING);
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
			}
			SamusStateManager::getInstance()->setOldStatus(eStatus::RUNNING);
			this->samus->setStatus(eStatus::INJURING);
			SamusStateManager::getInstance()->setOldState(this);

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
			
			SamusStateManager::getInstance()->setOldStatus(eStatus::RUNNING);
			this->samus->setStatus(eStatus::INJURING);
			SamusStateManager::getInstance()->setOldState(this);

			break;
		}

#pragma endregion
		default:
			break;
		}

		}
}
void SamusStateRunning::update(float dt)
{
	this->animation->update(dt);

	samus->updateHorizontal(dt);

	samus->updateVertical(dt);

	setBoundCollision();

	if (this->samus->isFaling())
	{
		this->samus->setAcrobat(false);
		this->samus->setFall(true);
		this->samus->setStatus(eStatus::JUMPING);
	}
	if (this->samus->getStatus() != eStatus::RUNNING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::JUMPING:

			//stop current animation
			this->animation->stop();

			break;
		case eStatus::STANDING:
			
			break;
		case eStatus::INJURING:

			break;
		default:
			break;
		}
		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}

	this->samus->setCanMoveLeft(true);
	this->samus->setCanMoveRight(true);
	this->samus->setFall(true);

	//this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);
}



void SamusStateRunning::onStart()
{
	runningNormal->setPause(false);
	runningUp->setPause(false);
	runningHittingRight->setPause(false);

	this->animation->start();
}

void SamusStateRunning::onExit()
{
	this->animation->stop();
}

void SamusStateRunning::fire()
{
	VECTOR2 stP;
	Bullet* bullet = BulletPool::getInstance()->getBullet();
	Sound::getInstance()->play(SOUND_BULLET, false);
	if (isUp)
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection(), this->samus->getPosition().y + this->samus->getSprite()->getHeight()*0.4f);
		bullet->setVelocity(VECTOR2(0, VELOCITY_BULLET));
	}
	else
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection()*this->samus->getSprite()->getWidth()*0.3f, this->samus->getPosition().y + 4);
		bullet->setVelocity(VECTOR2((float)VELOCITY_BULLET*this->samus->getDirection(), 0));
	}

	bullet->init(stP);
}
