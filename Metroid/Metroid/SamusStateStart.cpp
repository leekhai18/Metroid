#include "SamusStateStart.h"
#include "SamusStateManager.h"


SamusStateStart::SamusStateStart()
{
}

SamusStateStart::SamusStateStart(Samus * samus, Input* input) : BaseState(samus, input)
{
	samus->setOrigin(VECTOR2(0.5, 0.5));
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
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y - OFFSET_COLLISION_Y + 1);
	rect.left = position.x - WIDTH_COLLISION*0.5f;
	rect.right = position.x + WIDTH_COLLISION*0.5f;
	rect.top = position.y + HEIGHT_COLLISION*0.5f;
	rect.bottom = position.y - HEIGHT_COLLISION*0.5f;


	samus->setBoundCollision(rect);
}
void SamusStateStart::init()
{
	setBoundCollision();
	this->samus->setActiveBound();
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
			}
			if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
			{
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
