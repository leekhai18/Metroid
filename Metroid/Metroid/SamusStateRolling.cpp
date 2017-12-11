#include "SamusStateRolling.h"
#include "SamusStateManager.h"


SamusStateRolling::SamusStateRolling()
{
}

SamusStateRolling::SamusStateRolling(Samus * samus, Input * input) : BaseState(samus, input)
{
	timer = 0;
}


SamusStateRolling::~SamusStateRolling()
{

}

void SamusStateRolling::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y );
	rect.left = position.x - WIDTH_COLLISION*0.5f;
	rect.right = position.x + WIDTH_COLLISION*0.5f;
	rect.top = position.y + ROLL_HEIGHT*0.5f;
	rect.bottom = position.y - ROLL_HEIGHT*0.5f;
	samus->setBoundCollision(rect);
}

void SamusStateRolling::init()
{
	this->animation = samus->getRollingAnim();

	setBoundCollision();

	this->samus->setVelocityY(-SAMUS_MIN_SPEED_Y);

	isFall = true;
}

void SamusStateRolling::handleInput(float dt)
{
	//handle press left button
	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setFlipX(true);
			this->samus->setDirection(eDirection::left);
		}

		//check if moveLeft = false, it means samus is colliding with other object in right side
		if (this->samus->canMoveLeft() == true)
		{
			this->samus->setVelocityX(-SAMUS_VERLOCITY_X);
		}
	}

	//handle press right button
	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{

		if (this->samus->isInDirection(eDirection::left))
		{
			this->samus->setFlipX(false);
			this->samus->setDirection(eDirection::right);
		}

		//check if moveRight = false, it means samus is colliding with other object in left side
		if (this->samus->canMoveRight() == true)
		{
			this->samus->setVelocityX(SAMUS_VERLOCITY_X);
		}
	}

	if ((input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT)))
	{
		this->samus->setVelocityX(0);
	}

	if (input->isKeyDown(VK_UP) || input->isKeyDown(VK_X))
	{
		this->samus->setStatus(eStatus::STANDING);
	}
}

void SamusStateRolling::onCollision()
{
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
		case eID::WALL:
			switch (i->direction)
			{
			case CollideDirection::LEFT:
				this->samus->setVelocityX(0);
				//not allow move left
				this->samus->setCanMoveRight(false);
				break;
			case CollideDirection::RIGHT:
				this->samus->setVelocityX(0);
				//not allow move right
				this->samus->setCanMoveLeft(false);
				break;
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);

				this->samus->setPositionY(i->positionCollision + OFFSET_ROLLING);

				isFall = false;
				break;
			}
			break;
		default:
			break;
		}

	}
	
}
void SamusStateRolling::update(float dt)
{
	this->animation->update(dt);

	if (this->samus->canMoveLeft() || this->samus->canMoveRight())
	{
		this->samus->updateHorizontal(dt);
	}
	this->samus->updateVertical(dt);

	setBoundCollision();

	timer += dt;
	if (timer < 0.03f)
	{
		if (flagUpDown)
			this->samus->setPositionY(this->samus->getPosition().y - ACCELERATE_Y*dt);
	}
	else
	{
		flagUpDown = !flagUpDown;
		timer = 0;
	}

	if(isFall && !this->samus->isInStatus(eStatus::STANDING))
	{
		this->samus->setStatus(eStatus::FALLING_ROLLING);
	}

	if (!this->samus->isInStatus(eStatus::ROLLING))
	{
		switch (this->samus->getStatus())
		{
		case eStatus::STANDING:
			this->samus->setPositionY(this->samus->getPosition().y + OFFSET_STAND);
			break;
		default:
			break;
		}

		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}
}


void SamusStateRolling::onStart()
{
	this->animation->start();
}

void SamusStateRolling::onExit()
{
	this->animation->stop();
}
