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


	move_to_fall = true;
	canStanding = false;
}

void SamusStateRolling::handleInput(float dt)
{
	//handle press left button
	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setFlipX(true);
			//set direction to left
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
		case eID::WALL: case eID::BRICK:
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
				canStanding = true;
				reset_fall = true;
				move_to_fall = false;
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

	if(move_to_fall&&!this->samus->isInStatus(eStatus::STANDING))
	{
		this->samus->setStatus(eStatus::FALLING_ROLLING);
	}
	if (this->samus->getStatus() != eStatus::ROLLING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::STANDING:
			if(canStanding)
			{
				this->samus->setPositionY(this->samus->getPosition().y + OFFSET_STAND);
				SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			}
			break;
		case eStatus::FALLING_ROLLING:
			SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
			break;
		default:

			break;
		}
		//SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}
	this->samus->setCanMoveLeft(true);
	this->samus->setCanMoveRight(true);

	move_to_fall = true;
	this->samus->setVelocity(VECTOR2(0, -SAMUS_MIN_SPEED_Y));
	canStanding = false;
}


void SamusStateRolling::onStart()
{
	this->animation->start();
}

void SamusStateRolling::onExit()
{
	this->animation->stop();
}
