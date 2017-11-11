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
	if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
	{
		this->animation = samus->getJumpingAnim();
		this->samus->setPositionY(this->samus->getPosition().y - SAMUS_HEIGHT*0.5f);
		this->samus->setOrigin(VECTOR2(0.5f, 0.5f));
		return;
	}

	// Set Data for sprite
	this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpRight);
	this->samus->setOrigin(VECTOR2(0, 1.0f));
}

void SamusStateJumping::handleInput(float dt)
{

#pragma region Horizontal
	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{
		// Handle direction
		if (this->samus->isInDirection(eDirection::left))
		{
			this->samus->setScaleX(1);

			if (this->animation == nullptr)
				this->samus->setPositionX(this->samus->getPosition().x - this->samus->getSprite()->getWidth());

			this->samus->setDirection(eDirection::right);
		}

		// Handle horizontal
		this->samus->updateHorizontal(dt);
	}

	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		// Handle direction
		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setScaleX(-1);

			if (this->animation == nullptr)
				this->samus->setPositionX(this->samus->getPosition().x + this->samus->getSprite()->getWidth());

			this->samus->setDirection(eDirection::left);
		}

		// Handle horizontal
		this->samus->updateHorizontal(dt);
	}

	// Handle horizontal
	if (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT))
	{
		this->samus->updateHorizontal(dt);
	}
#pragma endregion

#pragma region Vertical
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
			// must fix because of origin sprite change
			if (animation != nullptr)
			{
				if (this->samus->isInDirection(eDirection::left))
					this->samus->setPositionX(this->samus->getPosition().x + SAMUS_WIDTH_RL_HALF);
				else if (this->samus->isInDirection(eDirection::right))
					this->samus->setPositionX(this->samus->getPosition().x - SAMUS_WIDTH_RL_HALF);
			}
		

			this->samus->setFall(false);
			this->samus->setStatus(eStatus::STANDING);
			SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
		}
	}
#pragma endregion


#pragma region Shooting

	if (this->animation == nullptr)
	{
		if (input->isKeyDown(VK_Z) && input->isKeyUp(VK_UP))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingAndJumpRight);
			this->samus->setOrigin(VECTOR2(0, 1.0f));

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
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingJumpUp);
			this->samus->setOrigin(VECTOR2(0, 1.0f));

			this->isUp = true;
			if (this->samus->timerShoot > TIME_SHOOTING)
			{
				this->fire();
				this->samus->timerShoot = 0;
			}
		}

		if (input->isKeyDown(VK_UP) && input->isKeyUp(VK_Z))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpUp);
			this->samus->setOrigin(VECTOR2(0, 1.0f));
		}

		if (input->isKeyUp(VK_Z) && input->isKeyUp(VK_UP) && this->samus->isInStatus(eStatus::JUMPING))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpRight);
			this->samus->setOrigin(VECTOR2(0, 1.0f));
		}
	}
#pragma endregion

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

void SamusStateJumping::fire()
{
	VECTOR2 stP;
	VECTOR2 tar;

	if (isUp)
	{
		if (this->samus->isInDirection(eDirection::right))
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.7f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);
		else if (this->samus->isInDirection(eDirection::left))
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.7f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);

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


