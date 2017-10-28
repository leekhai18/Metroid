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
}

void SamusStateRolling::handleInput(float dt)
{
	if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
	{
		this->samus->running(dt);
	}

	if (input->isKeyDown(VK_UP) || input->isKeyDown(VK_X))
	{
		this->samus->setStatus(eStatus::STANDING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
	}
}

void SamusStateRolling::update(float dt)
{
	this->animation->update(dt);
	this->handleInput(dt);
}

void SamusStateRolling::onStart()
{
	this->animation->start();
}

void SamusStateRolling::onExit()
{
	this->animation->stop();
}
