#include "SamusStateStart.h"
#include "SamusStateManager.h"


SamusStateStart::SamusStateStart()
{
}

SamusStateStart::SamusStateStart(Samus * samus, Input* input) : BaseState(samus, input)
{
	samus->setOrigin(VECTOR2(0, 1));
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
	if (this->samus->getDirection() == eDirection::right)
	{
		VECTOR2 position(this->samus->getPosition().x + WIDTH_RUN + OFFSET_STAND - OFFSET_RUN, samus->getPosition().y);
		rect.left = position.x - (WIDTH_COLLISION)+1;
		rect.right = position.x - 1;
		rect.top = position.y - (MAX_HEIHT)+1;
		rect.bottom = position.y - 1;

	}
	else
	{
		VECTOR2 position(this->samus->getPosition().x - WIDTH_RUN - OFFSET_STAND + OFFSET_RUN, samus->getPosition().y);
		rect.left = position.x + 1;
		rect.right = position.x + (WIDTH_COLLISION)-1;
		rect.top = position.y - (MAX_HEIHT)+1;
		rect.bottom = position.y - 1;

	}
	samus->setBoundCollision(rect);
}
void SamusStateStart::init()
{
	setBoundCollision();
	this->samus->setActiveBound(this->samus->getBoundCollision());
}

void SamusStateStart::handleInput(float dt)
{
	if (animation->isFinished())
	{
		if (input->isKeyDown(VK_RIGHT) || input->isKeyDown(VK_LEFT))
		{
			MetroidRect bound = samus->getBoundCollision();
			if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
			{
				samus->setDirection(eDirection::right);
				this->samus->setPositionX(this->samus->getPosition().x + WIDTH_RUN + OFFSET_STAND);
			}
			if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
			{
				this->samus->setPositionX(bound.left - 1 - OFFSET_RUN);
				//set flipX to true to turn left
				this->samus->setFlipX(true);
				//set direction to left
				this->samus->setDirection(eDirection::left);
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
