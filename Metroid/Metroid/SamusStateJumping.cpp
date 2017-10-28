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
		return;
	}

	// Set Data for sprite
	const SpriteData data = SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowJumpRight];
	this->samus->getSprite()->setSpriteDataRect(data.rect);
	this->samus->getSprite()->setSpriteWidth(data.width);
	this->samus->getSprite()->setSpriteHeigth(data.height);
}

void SamusStateJumping::handleInput(float dt)
{
	if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
	{
		this->samus->updateHorizontal(dt);
	}

	if (input->isKeyDown(VK_X))
	{
		this->samus->setFall(false);
		this->samus->updateVertical(dt);
	}

	if (input->isKeyUp(VK_X))
	{
		this->samus->setStatus(eStatus::FALLING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::FALLING);
		this->animation = nullptr;
	}
}

void SamusStateJumping::update(float dt)
{
	if (this->animation != nullptr)
		this->animation->update(dt);

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
		this->animation->stop();
}
