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
}

void SamusStateRunning::init()
{
	runningNormal = samus->getRunningNormalAnim();
	runningUp = samus->getRunningUpAnim();
	runningHittingRight = samus->getRunningHittingRightAnim();

	this->animation = runningNormal;
}

void SamusStateRunning::handleInput(float dt)
{
	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{
		// Handle horizontal
		this->samus->updateHorizontal(dt);

		// Handle direction
		if (this->samus->isInDirection(eDirection::left))
		{
			this->samus->setScaleX(1);
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
			this->samus->setPositionX(this->samus->getPosition().x + this->samus->getSprite()->getWidth());
			this->samus->setDirection(eDirection::left);
		}
	}

	// Handle horizontal
	if (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT))
	{
		this->samus->updateHorizontal(dt);
	}

	if (input->isKeyDown(VK_UP))
	{
		this->animation->stop();

		this->animation = runningUp;
		this->animation->start();
	}

	if (input->isKeyDown(VK_Z) && input->isKeyUp(VK_UP))
	{
		this->animation->stop();

		this->animation = runningHittingRight;
		this->animation->start();
	}

	if (input->isKeyDown(VK_UP) && input->isKeyDown(VK_Z))
	{
		this->animation->stop();

		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingUp);
		this->samus->setOrigin(VECTOR2(0, 1.0f));

		this->animation = runningUp;
		this->animation->start();
	}

	if (input->isKeyUp(VK_UP) && input->isKeyUp(VK_Z) && (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT)))
	{
		this->animation->stop();

		this->animation = runningNormal;
		this->animation->start();
	}

	if (input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{
		this->samus->setStatus(eStatus::STANDING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
	}

	if (input->isKeyDown(VK_X))
	{
		this->animation->stop();

		this->samus->setStatus(eStatus::JUMPING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::JUMPING);
	}
}

void SamusStateRunning::update(float dt)
{
	this->animation->update(dt);
}

void SamusStateRunning::onStart()
{
	this->animation->start();
}

void SamusStateRunning::onExit()
{
	this->animation->stop();
}
