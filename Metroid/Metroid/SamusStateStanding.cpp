#include "SamusStateStanding.h"
#include "SamusStateManager.h"
#include "BulletPool.h"

#define TIME_TO_RUNNING 0.07f

SamusStateStanding::SamusStateStanding()
{
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
	if (input->isKeyDown(VK_UP))
	{
		isUp = true;

		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnUp);
		this->samus->setOrigin(VECTOR2(0, 1.0f));

		return;
	}

	// Set Data for sprite
	this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
	this->samus->setOrigin(VECTOR2(0, 1.0f));

	timerToRunning = 0;
}

void SamusStateStanding::handleInput(float dt)
{
	if (input->isKeyUp(VK_LEFT) && input->isKeyUp(VK_RIGHT))
		timerToRunning = 0;

	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		timerToRunning += dt;
		if (timerToRunning > TIME_TO_RUNNING)
		{
			// must fix because of origin sprite change
			if (isUp)
				this->samus->setPositionX(this->samus->getPosition().x - SAMUS_WIDTH_UP_HALF);
			else
				this->samus->setPositionX(this->samus->getPosition().x - SAMUS_WIDTH_RL_HALF);

			this->samus->setStatus(eStatus::RUNNING);
			SamusStateManager::getInstance()->changeStateTo(eStatus::RUNNING);
			timerToRunning = 0;
		}

		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setScaleX(-1);
			this->samus->setPositionX(this->samus->getPosition().x + this->samus->getSprite()->getWidth());
			this->samus->setDirection(eDirection::left);
		}

		this->samus->updateHorizontal(dt);
	}

	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{
		timerToRunning += dt;
		if (timerToRunning > TIME_TO_RUNNING)
		{
			// must fix because of origin sprite change
			if (isUp)
				this->samus->setPositionX(this->samus->getPosition().x + SAMUS_WIDTH_UP_HALF);
			else
				this->samus->setPositionX(this->samus->getPosition().x + SAMUS_WIDTH_RL_HALF);

			this->samus->setStatus(eStatus::RUNNING);
			SamusStateManager::getInstance()->changeStateTo(eStatus::RUNNING);
			timerToRunning = 0;
		}

		if (this->samus->isInDirection(eDirection::left))
		{
			this->samus->setScaleX(1);
			this->samus->setPositionX(this->samus->getPosition().x - this->samus->getSprite()->getWidth());
			this->samus->setDirection(eDirection::right);
		}

		this->samus->updateHorizontal(dt);
	}

	if (input->isKeyDown(VK_X))
	{
		this->samus->setStatus(eStatus::JUMPING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::JUMPING);
	}

	if (input->isKeyDown(VK_UP))
	{
		isUp = true;

		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnUp);
		this->samus->setOrigin(VECTOR2(0, 1.0f));
	}

	if (isUp && input->isKeyUp(VK_UP))
	{
		isUp = false;
		
		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
		this->samus->setOrigin(VECTOR2(0, 1.0f));
	}

	if (input->isKeyDown(VK_DOWN))
	{
		this->samus->setStatus(eStatus::ROLLING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::ROLLING);
	}

	if (input->isKeyDown(VK_Z))
	{
		// Set up sprite Shooting
		if (isUp)
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingUp);
			this->samus->setOrigin(VECTOR2(0, 1.0f));
		}
		else
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingRight);
			this->samus->setOrigin(VECTOR2(0, 1.0f));
		}

		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
		}
	}
}

void SamusStateStanding::update(float dt)
{
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
	VECTOR2 tar;

	if (isUp)
	{
		if (this->samus->isInDirection(eDirection::right))
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.65f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);
		else if (this->samus->isInDirection(eDirection::left))
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.65f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);

		tar = VECTOR2(stP.x, stP.y - 100);
	}
	else
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.8f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			tar = VECTOR2(stP.x + 100, stP.y);
		}

		else if (this->samus->isInDirection(eDirection::left))
		{
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.8f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			tar = VECTOR2(stP.x - 100, stP.y);
		}
	}

	BulletPool::getInstance()->getBullet()->init(stP, tar);
}
