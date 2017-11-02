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
	const SpriteData *data = &(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowJumpRight]);
	this->samus->getSprite()->setSpriteDataRect(data->rect);
	this->samus->getSprite()->setSpriteWidth(data->width);
	this->samus->getSprite()->setSpriteHeigth(data->height);
	this->samus->setOrigin(VECTOR2(0, 1.0f));
}

void SamusStateJumping::handleInput(float dt)
{
	if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
	{
		this->samus->updateHorizontal(dt);
	}

	if (this->samus->isFaling() == false)
	{
		if (this->animation == nullptr)
		{
			if (input->isKeyDown(VK_Z))
			{
				// Set Data for sprite
				const SpriteData *data = &(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowHittingAndJumpRight]);
				this->samus->getSprite()->setSpriteDataRect(data->rect);
				this->samus->getSprite()->setSpriteWidth(data->width);
				this->samus->getSprite()->setSpriteHeigth(data->height);
				this->samus->setOrigin(VECTOR2(0, 1.0f));
			}

			if (input->isKeyDown(VK_UP))
			{
				// Set Data for sprite
				const SpriteData *data = &(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowHittingAndJumpUp]);
				this->samus->getSprite()->setSpriteDataRect(data->rect);
				this->samus->getSprite()->setSpriteWidth(data->width);
				this->samus->getSprite()->setSpriteHeigth(data->height);
				this->samus->setOrigin(VECTOR2(0, 1.0f));
			}
		}

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
}

void SamusStateJumping::update(float dt)
{
	if (this->animation != nullptr)
	{
		this->animation->update(dt);
	}

	this->handleInput(dt);
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
