#include "SamusStateStanding.h"
#include "SamusStateManager.h"


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
}

void SamusStateStanding::handleInput(float dt)
{
	if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
	{
		this->samus->setStatus(eStatus::RUNNING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::RUNNING);
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

		//this->fire();
	}

	if (input->isKeyUp(VK_Z) && !isUp && input->isKeyUp(VK_X))
	{
		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
		this->samus->setOrigin(VECTOR2(0, 1.0f));
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
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.7f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
		else if (this->samus->isInDirection(eDirection::left))
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.7f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);

		tar = VECTOR2(stP.x, stP.y - 100);
	}
	else
	{
		if (this->samus->isInDirection(eDirection::right))
		{

			tar = VECTOR2(stP.x + 100, stP.y);
		}

		else if (this->samus->isInDirection(eDirection::left))
		{
			tar = VECTOR2(stP.x - 100, stP.y);
		}
	}

	this->samus->getBullet()->init(stP, tar);
}
