#include "SamusStateJumping.h"
#include "SamusStateManager.h"


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
	if (this->samus->getDirection() == eDirection::right)
	{
		VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y);
		rect.left = position.x - (WIDTH_COLLISION)+1;
		rect.right = position.x - 1;
		rect.top = position.y + 1;
		rect.bottom = position.y + (JUMP_HEIGHT)-1;

	}
	else
	{
		VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y);
		rect.left = position.x + 1;
		rect.right = position.x + (WIDTH_COLLISION)-1;
		rect.top = position.y + 1;
		rect.bottom = position.y + (JUMP_HEIGHT)-1;
	}
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
	// Set Data for sprite
	if (samus->getDirection() == eDirection::left)
	{
		this->samus->setOrigin(VECTOR2(0, 0));
	}
	else
	{
		this->samus->setOrigin(VECTOR2(1, 0));
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

			if (this->animation == nullptr)
			{
				//When change side boundCollision not change
				MetroidRect bound = samus->getBoundCollision();
				//  Hand of samus   BoundCollision
				//	  	 /    _____/
				//	   _/___ /_			     ___ _
				//	  |_|	|  |		    |	|_|
				//	1	|	|  |	-> 		|	|  2
				//		|___|  |			|___|
				//pos__________|____orgin
				//	  |_ ___ _	 
				// 2    |	|_|	
				//		|	|  		
				//		|___| 
				//       
				// Note: 1 left, 2 right
				//when side of samus is right, set position.x = bound.left - 1

				this->samus->setPositionX(bound.right + 1);
				this->samus->setOrigin(VECTOR2(1, 0));
				//change to orgin (0,1)

				this->samus->setDirection(eDirection::right);
			}

			this->samus->setVelocityX(0);

		}
		//set velocity to move right

		this->samus->setVelocityX(this->samus->getVelocity().x + ACCELERATE_X*dt);
		if (this->samus->getVelocity().x > SAMUS_VERLOCITY_X)
		{
			this->samus->setVelocityX(SAMUS_VERLOCITY_X);
		}
	}

	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{

		// Handle direction	
		if (this->samus->isInDirection(eDirection::right))
		{
			this->samus->setFlipX(true);

			if (this->animation == nullptr)
			{
				//When change side boundCollision not change
				MetroidRect bound = samus->getBoundCollision();
				//BoundCollision  Hand of samus
				//	  \			/
				//	  _\ ___ _ /	   _ ___
				//	 |  |	|_|		  |_|	|
				//	1|	|	|	-> 		|	| 2
				//	 |	|___|			|___|
				//Pos|___________ Orgin
				//	   _ ___ ___/
				// 2  |_|	|	
				//		|	|  		
				//		|___|
				//
				// Note: 1 right, 2 left
				//Position of samus 2 is on bottom right because we flipX samus 1 with orgin( 0, 1). 
				//And then we change to orgin(0,1) 
				//
				//FlipX with orgin(0,1) when samus side is right
				//After FlipX     Before flipX
				//		   \_ __ __ _/
				//		   |_|	|  |_|
				//			 |	|  |
				//			 |__|__|
				//				|
				//   		position	
				//so to not change boundCollision when side of samus is left, set position.x = bound.right +1
				this->samus->setPositionX(bound.left - 1);
				//change orgin to (1,1)
				this->samus->setOrigin(VECTOR2(0, 0));
				//set direction to left
				this->samus->setDirection(eDirection::left);
			}
			this->samus->setVelocityX(0);
		}

		//set velocity to move left
		this->samus->setVelocityX(this->samus->getVelocity().x - ACCELERATE_X*dt);
		if (-this->samus->getVelocity().x > SAMUS_VERLOCITY_X)
		{
			this->samus->setVelocityX(-SAMUS_VERLOCITY_X);
		}
	}

	// Handle horizontal
	//if (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT))
	//{
	//	//this->samus->updateHorizontal(dt);
	//	//this->samus->setVelocityX(-SAMUS_VERLOCITY_X);
	//}
#pragma endregion

#pragma region Vertical
	// Handle vertical
	if (this->samus->isFaling() == false)
	{
		//assign velocity for samus to change velocity
		//initial velocity(V_0)
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

		//distance = V*dt
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
			this->samus->setFall(true);

			this->samus->setVelocityY(this->samus->getVelocity().y*-1.0f);

			this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);
		}
		//if we release jump button, samus will jump to MIN_JUMP
		if (input->isKeyUp(VK_X))
		{
			if (jumpDistance >= MIN_JUMP)
			{
				this->samus->setFall(true);
				this->samus->setVelocityY(this->samus->getVelocity().y*-1.0f);

				this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);
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
		case eID::WALL:
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

				if (animation != NULL)
				{
					addY = addY + HEIGHT_RUN*0.5f;
				}
				this->samus->setPositionY(addY);
				break;
			}
			break;
			//another object
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

			this->samus->setPositionY(positionCollide);
			//we add 
			if (this->samus->isInDirection(eDirection::right))
			{
				this->samus->setPositionX(this->samus->getPosition().x + OFFSET_RUN);
			}
			else
			{
				this->samus->setPositionX(this->samus->getPosition().x - OFFSET_RUN);
			}
			break;
		case eStatus::ACROBAT:
			if (this->samus->isInDirection(eDirection::right))
			{
				this->samus->setPosition(VECTOR2(this->samus->getPosition().x - WIDTH_JUMP*0.5f + OFFSET_ACROBAT, this->samus->getPosition().y + HEIGHT_JUMP*0.5f));
			}
			else
			{
				this->samus->setPosition(VECTOR2(this->samus->getPosition().x + WIDTH_JUMP*0.5f - OFFSET_ACROBAT, this->samus->getPosition().y + HEIGHT_JUMP*0.5f));
			}
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
		if (this->samus->isInDirection(eDirection::right))
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.7f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);
		else if (this->samus->isInDirection(eDirection::left))
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.7f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);

		bullet->setVelocity(VECTOR2(0, -VELOCITY));
	}
	else
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.8f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			bullet->setVelocity(VECTOR2(VELOCITY, 0));
		}

		else if (this->samus->isInDirection(eDirection::left))
		{
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.8f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			bullet->setVelocity(VECTOR2(-VELOCITY, 0));
		}
	}

	bullet->init(stP);
}


