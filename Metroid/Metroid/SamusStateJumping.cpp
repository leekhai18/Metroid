#include "SamusStateJumping.h"
#include "SamusStateManager.h"
#include "GameLog.h"

SamusStateJumping::SamusStateJumping()
{
}

SamusStateJumping::SamusStateJumping(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateJumping::~SamusStateJumping()
{
}

void SamusStateJumping::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y );
	rect.left = position.x - JUMP_WIDTH*0.5f;
	rect.right = position.x + JUMP_WIDTH*0.5f;
	rect.top = position.y + JUMP_HEIGHT*0.5f;
	rect.bottom = position.y - JUMP_HEIGHT*0.5f;
	samus->setBoundCollision(rect);
}
void SamusStateJumping::init()
{
	time = 0;
	if (this->samus->isFaling() == false)
	{
		this->samus->setVelocityY(SAMUS_MAX_SPEED_Y);
		velocity_frame = SAMUS_MAX_SPEED_Y;
	}
	
	this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpRight);
	setBoundCollision();
}

void SamusStateJumping::handleInput(float dt)
{

#pragma region Horizontal
	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{
		// Handle direction
		if (this->samus->isInDirection(eDirection::left))
		{
			this->samus->setFlipX(false);
			this->samus->setDirection(eDirection::right);
			this->samus->setVelocityX(0);

		}

		this->samus->setVelocityX(SAMUS_VELOCITY_JUMP_X);
	}

	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{

		// Handle direction	
		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setFlipX(true);

				
			this->samus->setDirection(eDirection::left);

			this->samus->setVelocityX(0);
		}

		this->samus->setVelocityX(-SAMUS_VELOCITY_JUMP_X);
	}

#pragma endregion

#pragma region Vertical
	// Handle vertical
	if (this->samus->isFaling() == false)
	{

		this->samus->setVelocityY(velocity_frame);

		//set velocity.y to jump up
		//V= V_0 + a*dt;
		//a: accelerate is the rate of change of velocity of an object with respect to time
		//so if a increase V also increase

		//calculate velocity after dt(s) (V)
		velocity_frame = velocity_frame + ACCELERATE_Y*dt;

		//and then set average velocity.y 
		//samus.velocity.y= (V + V_0)/2
		this->samus->setVelocityY((velocity_frame + this->samus->getVelocity().y) / 2);

		float distance = this->samus->getVelocity().y*dt;
		jumpDistance += distance;
		time += dt;

		//if acrobat and time enough to change acrobat state
		if (time >= TIME_TO_ACROBAT&&samus->isAcrobat())
		{
			samus->setStatus(eStatus::ACROBAT);
		}

		if (jumpDistance < MAX_JUMP)
		{
			this->samus->setFall(false);
		}
		else
		{
			jumpDistance = 0;
			if (this->samus->isFaling() == false)
			{
				this->samus->setFall(true);
				this->samus->setVelocityY(SAMUS_V0_FALL_Y);
			}

			this->samus->setVelocityY(this->samus->getVelocity().y*-1.0f + ACCELERATE_Y*dt);
		}
		//if we release jump button, samus will jump to MIN_JUMP
		if (input->isKeyUp(VK_X))
		{
			if (jumpDistance >= MIN_JUMP)
			{
				if (this->samus->isFaling() == false)
				{
					this->samus->setFall(true);
					this->samus->setVelocityY(SAMUS_V0_FALL_Y);
				}

				this->samus->setVelocityY(this->samus->getVelocity().y*-1.0f + ACCELERATE_Y*dt);
			}
		}
	}
	else
	{
		this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);
	}
#pragma endregion


#pragma region Shooting

	if (this->animation == nullptr)
	{
		if (input->isKeyDown(VK_Z) && input->isKeyUp(VK_UP))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingAndJumpRight);

			this->isUp = false;
			if (this->samus->timerShoot > TIME_SHOOTING)
			{
				this->fire();
				this->samus->timerShoot = 0;
			}
		}

		if (input->isKeyDown(VK_Z) && input->isKeyDown(VK_UP))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingJumpUp);

			this->isUp = true;
			if (this->samus->timerShoot > TIME_SHOOTING)
			{
				this->fire();
				this->samus->timerShoot = 0;
			}
		}

		if (input->isKeyDown(VK_UP) && input->isKeyUp(VK_Z))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpUp);
		}

		if (input->isKeyUp(VK_Z) && input->isKeyUp(VK_UP) && this->samus->isInStatus(eStatus::JUMPING))
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpRight);
		}
	}
#pragma endregion

}
#pragma region Collision
void SamusStateJumping::onCollision()
{
	float addX = 0, addY = 0;

	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
			//handle collision with wall
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
				this->samus->setJump(false);
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
				this->samus->setVelocityY(0);

				this->samus->setPositionY(addY - OFFSET_JUMP);
				break;
			}
			break;
			//another object
		case eID::SKREE:
			GAMELOG("VA CHAM SKREE");
			break;

		default:
			break;
		}

	}

}
#pragma endregion

void SamusStateJumping::update(float dt)
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
	if (this->samus->getStatus() != eStatus::JUMPING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::STANDING:

			this->samus->setPositionY(positionCollide +  OFFSET_STAND);

			break;
		case eStatus::ACROBAT:
			this->samus->setVelocityX(0);
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



void SamusStateJumping::onStart()
{
	if (this->animation != nullptr)
		this->animation->start();
}

void SamusStateJumping::onExit()
{
	if (this->animation != nullptr)
	{
		this->animation->stop();
		this->animation = nullptr;
	}
}

void SamusStateJumping::fire()
{
	VECTOR2 stP;
	Bullet* bullet = BulletPool::getInstance()->getBullet();

	if (isUp)
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection(), this->samus->getPosition().y + this->samus->getSprite()->getHeight()*0.5f);
		bullet->setVelocity(VECTOR2(0, VELOCITY));
	}
	else
	{
		stP = VECTOR2(this->samus->getPosition().x + this->samus->getDirection()*this->samus->getSprite()->getWidth()*0.3f, this->samus->getPosition().y + 2);
		bullet->setVelocity(VECTOR2((float)VELOCITY*this->samus->getDirection(), 0));
	}

	bullet->init(stP);
}


