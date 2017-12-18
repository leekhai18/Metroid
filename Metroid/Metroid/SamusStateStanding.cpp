#include "SamusStateStanding.h"
#include "SamusStateManager.h"
#include "BulletPool.h"
#include "GameLog.h"

#define TIME_TO_RUNNING 0.1f

SamusStateStanding::SamusStateStanding()
{
}
void SamusStateStanding::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y - OFFSET_COLLISION_Y + 1);
	rect.left = position.x - WIDTH_COLLISION*0.5f;
	rect.right = position.x + WIDTH_COLLISION*0.5f;
	rect.top = position.y + HEIGHT_COLLISION*0.5f;
	rect.bottom = position.y - HEIGHT_COLLISION*0.5f;
	samus->setBoundCollision(rect);
}
SamusStateStanding::SamusStateStanding(Samus * samus, Input * input) : BaseState(samus, input)
{
	isUp = false;
}

SamusStateStanding::~SamusStateStanding()
{
}

void SamusStateStanding::init()
{
	this->samus->setVelocityX(0);
	if (this->samus->isFaling())
		this->samus->setVelocityY(SAMUS_V0_FALL_Y);
	else
		this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);

	setBoundCollision();
	timerToRunning = 0;
	if (input->isKeyDown(VK_UP))
	{
		isUp = true;
		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnUp);
		return;
	}

	// Set Data for sprite
	this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
	canRolling = false;
}

void SamusStateStanding::handleInput(float dt)
{
	this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);

	if (input->isKeyUp(VK_LEFT) && input->isKeyUp(VK_RIGHT))
		timerToRunning = 0;

	//handle press left button
	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setFlipX(true);
			//set direction to left
			this->samus->setDirection(eDirection::left);
		}

		//check if moveLeft = false, it means samus is colliding with other object in right side
		if (this->samus->canMoveLeft() == true)
		{
			timerToRunning += dt;
			//when timeToRunning enough, we change to running state
			if (timerToRunning > TIME_TO_RUNNING)
			{
				//change state to runnings
				this->samus->setStatus(eStatus::RUNNING);
				//we set jump to false, when we realease jump button it will change true
				this->samus->setCanJump(false);
				//reset timer
				timerToRunning = 0;
				return;
			}
		}
	}

	//handle press right button
	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{

		if (this->samus->isInDirection(eDirection::left))
		{
			//flipX change to false to turn right
			this->samus->setFlipX(false);
			this->samus->setDirection(eDirection::right);

		}

		//check if moveRight = false, it means samus is colliding with other object in left side
		if (this->samus->canMoveRight() == true)
		{
			timerToRunning += dt;
			//when timeToRunning enough, we change to running state
			if (timerToRunning > TIME_TO_RUNNING)
			{
				//we set jump to false, when we realease jump button it will change true
				this->samus->setCanJump(false);
				//change state to runnings
				this->samus->setStatus(eStatus::RUNNING);
				//reset timer
				timerToRunning = 0;
				return;
			}
		}
	}

	//reset jump when user release jump button
	if (input->isKeyUp(VK_X))
	{
		this->samus->setCanJump(true);
	}

	if (input->isKeyDown(VK_X) && this->samus->canJump() == true)
	{
		this->samus->setFall(false);
		this->samus->setAcrobat(false);
		this->samus->setVelocityX(0);
		this->samus->setStatus(eStatus::JUMPING);
		return;
	}

	if (input->isKeyDown(VK_UP))
	{
		isUp = true;

		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnUp);
	}

	if (isUp && input->isKeyUp(VK_UP))
	{
		isUp = false;

		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
	}

	if (input->isKeyDown(VK_DOWN) && this->samus->isHaveMariMaru())
	{
		this->samus->setVelocityX(0);
		this->samus->setStatus(eStatus::ROLLING);
		return;
	}

	if (!isUp && input->isKeyUp(VK_Z) && isShoot)
	{
		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
		isShoot = false;
	}

	if (input->isKeyDown(VK_Z))
	{
		// Set up sprite Shooting
		if (isUp)
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingUp);
		}
		else
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingRight);
		}

		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
			this->isShoot = true;
		}
	}
	
}

void SamusStateStanding::onCollision(float dt)
{
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
		case eID::WALL:case eID::BRICK:
			switch (i->direction)
			{
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);
				this->samus->setPositionY(i->positionCollision + OFFSET_STAND);

				canRolling = true;
				break;

			}
			break;

		case eID::SKREE:
			GAMELOG("VA CHAM SKREE");
			SamusStateManager::getInstance()->setOldStatus(eStatus::STANDING);
			this->samus->setStatus(eStatus::INJURING);
			SamusStateManager::getInstance()->setOldState(this);
			break;

		case eID::ZOMMER:
			switch (i->direction)
			{
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);
				break;
			}
			//this->samus->setVelocity(VECTOR2(0, 0));
			SamusStateManager::getInstance()->setOldStatus(eStatus::STANDING);
			this->samus->setStatus(eStatus::INJURING);
			SamusStateManager::getInstance()->setOldState(this);
			break;
		default:
			break;
		}
	}
}

void SamusStateStanding::update(float dt)
{

	this->samus->updateVertical(dt);
	setBoundCollision();

	if (this->samus->getStatus() != eStatus::STANDING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::JUMPING:
		
			//this->samus->setPositionX(this->samus->getPosition().x )
			SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			break;
		case eStatus::RUNNING:
			SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			break;
		case eStatus::ROLLING:
			if(canRolling)
			{
				SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			}			
			break;
		case eStatus::INJURING:
			SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			break;
		default:
			break;
		}

		//SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}

	canRolling = false;
}

void SamusStateStanding::onStart()
{
}

void SamusStateStanding::onExit()
{
}

void SamusStateStanding::fire()
{
	VECTOR2 stP;
	Bullet* bullet = BulletPool::getInstance()->getBullet();

	if (isUp)
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection(), this->samus->getPosition().y + this->samus->getSprite()->getHeight()*0.4f);
		bullet->setVelocity(VECTOR2(0, VELOCITY));
	}
	else
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection()*this->samus->getSprite()->getWidth()*0.3f, this->samus->getPosition().y + 2);
		bullet->setVelocity(VECTOR2((float)VELOCITY*this->samus->getDirection(), 0));
	}

	bullet->init(stP);
}
