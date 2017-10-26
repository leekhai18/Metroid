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
	if (input->isKeyDown(VK_RIGHT) || input->isKeyDown(VK_LEFT))
	{
		SamusStateManager::getInstance()->changeStateTo(eStatus::RUNNING);
		samus->running(dt);
	}

	if (input->isKeyDown(VK_UP))
	{
		SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
	}

	if (input->isKeyDown(VK_X))
	{
		SamusStateManager::getInstance()->changeStateTo(eStatus::JUMPING);
	}
}

void SamusStateStart::update(float dt)
{
	animation->update(dt);

	if (animation->isFinished())
	{
		handleInput(dt);
	}
}

void SamusStateStart::onStart()
{
}

void SamusStateStart::onExit()
{
}
