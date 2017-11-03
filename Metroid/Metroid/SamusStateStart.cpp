#include "SamusStateStart.h"
#include "SamusStateManager.h"


SamusStateStart::SamusStateStart()
{
}

SamusStateStart::SamusStateStart(Samus * samus, Input* input) : BaseState(samus, input)
{
	animation = samus->getStartingAnim();
	animation->start();
}

SamusStateStart::~SamusStateStart()
{
}

void SamusStateStart::init()
{
}

void SamusStateStart::handleInput(float dt)
{
	if (animation->isFinished())
	{
		if (input->isKeyDown(VK_RIGHT) || input->isKeyDown(VK_LEFT))
		{
			this->samus->setStatus(eStatus::RUNNING);
			SamusStateManager::getInstance()->changeStateTo(eStatus::RUNNING);
		}

		if (input->isKeyDown(VK_UP) || input->isKeyDown(VK_Z))
		{
			this->samus->setStatus(eStatus::STANDING);
			SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
		}

		if (input->isKeyDown(VK_X))
		{
			this->samus->setStatus(eStatus::JUMPING);
			SamusStateManager::getInstance()->changeStateTo(eStatus::JUMPING);
		}
	}
}

void SamusStateStart::update(float dt)
{
	animation->update(dt);
}

void SamusStateStart::onStart()
{
}

void SamusStateStart::onExit()
{
	this->animation->stop();
}
