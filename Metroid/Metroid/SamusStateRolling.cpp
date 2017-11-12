#include "SamusStateRolling.h"
#include "SamusStateManager.h"


SamusStateRolling::SamusStateRolling()
{
}

SamusStateRolling::SamusStateRolling(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateRolling::~SamusStateRolling()
{
}

void SamusStateRolling::init()
{
	this->animation = samus->getRollingAnim();
	this->samus->setPosition(this->samus->getPosition().x + SAMUS_WIDTH_RL_HALF, this->samus->getPosition().y - SAMUS_HEIGHT*0.5f);
}

void SamusStateRolling::handleInput(float dt)
{
	if (input->isKeyDown(VK_UP) || input->isKeyDown(VK_X))
	{
		this->samus->setStatus(eStatus::STANDING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
		return;
	}

	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{
		// Handle direction
		if (this->samus->isInDirection(eDirection::left))
		{
			this->samus->setScaleX(1);
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
}

void SamusStateRolling::update(float dt)
{
	this->animation->update(dt);
}

void SamusStateRolling::onStart()
{
	this->animation->start();
}

void SamusStateRolling::onExit()
{
	this->animation->stop();
}
