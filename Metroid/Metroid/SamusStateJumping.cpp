#include "SamusStateJumping.h"
#include "SamusStateManager.h"


SamusStateJumping::SamusStateJumping()
{
}

SamusStateJumping::SamusStateJumping(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateJumping::~SamusStateJumping()
{
}

void SamusStateJumping::init()
{
	if ( input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT) )
	{
		this->animation = samus->getJumpingAnim();
		this->samus->setPosition(this->samus->getPosition().x + 10, this->samus->getPosition().y - 15);
		return;
	}

	// Set Data for sprite
	this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpRight);
	this->samus->setOrigin(VECTOR2(0, 1.0f));
}

void SamusStateJumping::handleInput(float dt)
{
	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{
		// Handle horizontal
		this->samus->updateHorizontal(dt);

		// Handle direction
		if (this->samus->isInDirection(eDirection::left))
		{
			this->samus->setScaleX(1);

			if (this->animation == nullptr)
				this->samus->setPositionX(this->samus->getPosition().x - this->samus->getSprite()->getWidth());

			this->samus->setDirection(eDirection::right);
		}
	}

	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		// Handle horizontal
		this->samus->updateHorizontal(dt);

		// Handle direction
		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setScaleX(-1);

			if (this->animation == nullptr)
				this->samus->setPositionX(this->samus->getPosition().x + this->samus->getSprite()->getWidth());

			this->samus->setDirection(eDirection::left);
		}
	}

	// Handle horizontal
	if (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT))
	{
		this->samus->updateHorizontal(dt);
	}

	// Handle vertical
	if (this->samus->isFaling() == false)
	{
		if (input->isKeyDown(VK_X))
		{
			this->samus->setFall(false);
			this->samus->updateVertical(dt);
		}

		if (input->isKeyUp(VK_X))
		{
			this->samus->setFall(true);
		}
	}
	else
	{
		this->samus->updateVertical(dt);

		// check if samus is on ground => you would set changetoState(Standing) by collision
		if (this->samus->getPosition().y >= GAME_HEIGHT*0.8) // just test
		{
			this->samus->setFall(false);
			this->samus->setStatus(eStatus::STANDING);
			SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
		}
	}

	if (this->animation == nullptr)
	{
		if (input->isKeyDown(VK_Z) && input->isKeyUp(VK_UP))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingAndJumpRight);
			this->samus->setOrigin(VECTOR2(0, 1.0f));
		}

		if (input->isKeyDown(VK_UP) && input->isKeyUp(VK_Z))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpUp);
			this->samus->setOrigin(VECTOR2(0, 1.0f));
		}

		if (input->isKeyDown(VK_Z) && input->isKeyDown(VK_UP))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingJumpUp);
			this->samus->setOrigin(VECTOR2(0, 1.0f));
		}

		if (input->isKeyUp(VK_Z) && input->isKeyUp(VK_UP))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpRight);
			this->samus->setOrigin(VECTOR2(0, 1.0f));
		}
	}
}

void SamusStateJumping::update(float dt)
{
	if (this->animation != nullptr)
	{
		this->animation->update(dt);
	}
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
