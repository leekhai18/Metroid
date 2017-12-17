#include "SamusStateAcrobat.h"
#include "SamusStateManager.h"


SamusStateAcrobat::SamusStateAcrobat()
{
}

SamusStateAcrobat::SamusStateAcrobat(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateAcrobat::~SamusStateAcrobat()
{
}

void SamusStateAcrobat::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y );
	rect.left = position.x - WIDTH_COLLISION*0.5f;
	rect.right = position.x + WIDTH_COLLISION*0.5f;
	rect.top = position.y + JUMP_HEIGHT*0.5f;
	rect.bottom = position.y - JUMP_HEIGHT*0.5f;
	samus->setBoundCollision(rect);
}
void SamusStateAcrobat::init()
{
	this->animation = samus->getJumpingAnim();
	time = 0;
	this->position_to_jump = this->samus->getPosition().y;
	setBoundCollision();
}

void SamusStateAcrobat::handleInput(float dt)
{
#pragma region Horizontal
	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{
		// Handle direction
		if (this->samus->isInDirection(eDirection::left))
		{
			this->samus->setFlipX(false);
			this->samus->setDirection(eDirection::right);
		}
		this->samus->setVelocityX(SAMUS_VELOCITY_JUMP_X);
	}

	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setFlipX(true);
			this->samus->setDirection(eDirection::left);
		}

		this->samus->setVelocityX(-SAMUS_VELOCITY_JUMP_X);
	}

	if ((input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT)))
	{
		this->samus->setVelocityX(0);
	}
#pragma endregion

#pragma region Vertical
	// Handle vertical
	if (this->samus->isFaling() == false) // is going up
	{	
		//samus.velocity.y= V + at
		this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);

		jumpDistance += this->samus->getVelocity().y*dt;
		time += dt;
		
		if (jumpDistance < MAX_JUMP)
		{
			this->samus->setFall(false);
		}
		else
		{
			jumpDistance = 0;
			this->samus->setFall(true);
			this->samus->setVelocityY(SAMUS_V0_FALL_Y);
		}

		//if we release jump button, samus will jump to MIN_JUMP
		if (input->isKeyUp(VK_X))
		{
			this->samus->setFall(true);
			this->samus->setVelocityY(SAMUS_V0_FALL_Y);		
		}
	}
	else //Falling
	{
		if (this->samus->getVelocity().y > -SAMUS_MAX_SPEED_Y)
			this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);

		if(this->samus->getPosition().y <= position_to_jump)
			this->samus->setStatus(eStatus::JUMPING);
	}
#pragma endregion
}

#pragma region Collision
void SamusStateAcrobat::onCollision()
{
	float addX = 0, addY = 0;

	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
#pragma region Wall
		case eID::WALL: case eID::BRICK:
			switch (i->direction)
			{
			case CollideDirection::LEFT:
				this->samus->setCanMoveRight(false);
				this->samus->setVelocityX(0);
				break;

			case CollideDirection::RIGHT:
				this->samus->setCanMoveLeft(false);
				this->samus->setVelocityX(0);
				break;

			case CollideDirection::TOP:
				jumpDistance = 0;
				//set jump = false, when user release jump button set to true
				this->samus->setCanJump(false);
				//set fall to false
				this->samus->setFall(false);
				//reset velocity
				this->samus->setVelocityY(0);
				positionCollide = i->positionCollision;
				this->samus->setStatus(eStatus::STANDING);
				break;

			case CollideDirection::BOTTOM:
				jumpDistance = 0;
				this->samus->setFall(true);
				this->samus->setVelocityY(0);
				addY = i->positionCollision;
				this->samus->setPositionY(addY - OFFSET_JUMP);
				break;
			}

			break;
#pragma endregion

			//another object
		case eID::ZOMMER:
			switch (i->direction)
			{
			case CollideDirection::LEFT:
				this->samus->setVelocityX(0);
				//not allow move left
				//this->samus->setCanMoveRight(false);
				break;
			case CollideDirection::RIGHT:
				this->samus->setVelocityX(0);
				//not allow move right
				//this->samus->setCanMoveLeft(false);
				break;
			case CollideDirection::TOP:
				this->samus->setVelocityY(0);
				break;
			case CollideDirection::BOTTOM:
				this->samus->setVelocityY(0);
				break;
			}
			//this->samus->setVelocity(VECTOR2(0, 0));
			SamusStateManager::getInstance()->setOldStatus(eStatus::ACROBAT);
			SamusStateManager::getInstance()->setOldState(this);
			this->samus->setStatus(eStatus::INJURING);
			break;
		default:
			break;
		}
	}
}
#pragma endregion

void SamusStateAcrobat::update(float dt)
{
	if (this->animation != nullptr)
	{
		this->animation->update(dt);
	}
	//update position.x
	if (this->samus->canMoveLeft() || this->samus->canMoveRight())
	{
		this->samus->updateHorizontal(dt);
	}
	//update position.y
	this->samus->updateVertical(dt);
	setBoundCollision();

#pragma region ChangeState
	if (this->samus->getStatus() != eStatus::ACROBAT)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::JUMPING:

			break;
		case eStatus::STANDING:
			this->samus->setPositionY(positionCollide + OFFSET_STAND);
			//this->samus->setJump(false);
			break;
		default:
			break;
		}
		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}
#pragma endregion

	this->samus->setCanMoveLeft(true);
	this->samus->setCanMoveRight(true);

}



void SamusStateAcrobat::onStart()
{
	this->animation->start();
}

void SamusStateAcrobat::onExit()
{
	this->animation->stop();
	this->animation = nullptr;	
}



