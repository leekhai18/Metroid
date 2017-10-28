#include "SamusStateRunning.h"
#include "SamusStateManager.h"


SamusStateRunning::SamusStateRunning()
{
}

SamusStateRunning::SamusStateRunning(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateRunning::~SamusStateRunning()
{
	runningNormal = nullptr;
	runningShoot = nullptr;
	runningUp = nullptr;

	delete runningNormal, runningShoot, runningUp;
}

void SamusStateRunning::init()
{
	runningNormal = samus->getRunningNormalAnim();
	runningUp = samus->getRunningUpAnim();
	runningShoot = samus->getRunningShootAnim();

	this->animation = runningNormal;
}

void SamusStateRunning::handleInput(float dt)
{
	if (input->isKeyDown(VK_RIGHT))
	{
		this->handleLR(dt);
	} 
	else if (input->isKeyDown(VK_LEFT))
	{
		this->handleLR(dt);
	}

	if (input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{
		this->samus->setStatus(eStatus::STANDING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
	}
}

void SamusStateRunning::update(float dt)
{
	this->animation->update(dt);
	handleInput(dt);
}

void SamusStateRunning::onStart()
{
	this->animation->start();
}

void SamusStateRunning::onExit()
{
	this->animation->stop();
}

void SamusStateRunning::handleLR(float dt)
{
	samus->running(dt);

	if (input->isKeyDown(VK_UP))
	{
		this->animation->stop();

		this->animation = runningUp;
		this->animation->start();
	}

	if (input->isKeyDown(VK_X))
	{
		this->animation->stop();

		//this->samus->setStatus(eStatus::JUMPING);
		//SamusStateManager::getInstance()->changeStateTo(eStatus::JUMPING);
	}

	if (input->isKeyDown(VK_Z))
	{
		this->animation->stop();

		this->animation = runningShoot;
		this->animation->start();
	}

	if (input->isKeyUp(VK_UP) && input->isKeyUp(VK_Z) && ( input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT) ))
	{
		this->animation->stop();

		this->animation = runningNormal;
		this->animation->start();
	}
}
