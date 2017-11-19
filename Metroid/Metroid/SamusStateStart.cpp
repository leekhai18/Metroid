#include "SamusStateStart.h"
#include "SamusStateManager.h"


SamusStateStart::SamusStateStart()
{
}

SamusStateStart::SamusStateStart(Samus * samus, Input* input) : BaseState(samus, input)
{
	samus->setOrigin(VECTOR2(0, 1.0f));
	animation = samus->getStartingAnim();
	init();
	animation->start();
}

SamusStateStart::~SamusStateStart()
{
}
void SamusStateStart::setBoundCollision()
{
	MetroidRect rect;
	rect.left = samus->getPosition().x + 1;
	rect.right = samus->getPosition().x + (MAX_WIDTH)-1;
	rect.top = samus->getPosition().y - (MAX_HEIHT)+1;
	rect.bottom = samus->getPosition().y - 1;
	this->samus->setBoundCollision(rect);
}
void SamusStateStart::init()
{
	setBoundCollision();
}

void SamusStateStart::handleInput(float dt)
{
	if (animation->isFinished())
	{
		if (input->isKeyDown(VK_RIGHT) || input->isKeyDown(VK_LEFT))
		{
			MetroidRect bound = samus->getBoundCollision();

			if(input->isKeyDown(VK_RIGHT))
			{
				samus->setDirection(eDirection::right);
			}
			else
			{
				samus->setDirection(eDirection::left);
				this->samus->setPositionX(bound.right + 1);
				this->samus->setFlipX(true);
			}

			this->samus->setStatus(eStatus::STANDING);
			SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
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
