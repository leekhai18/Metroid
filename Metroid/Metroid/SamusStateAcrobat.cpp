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

	if (this->samus->getDirection() == eDirection::right)
	{
		VECTOR2 position(this->samus->getPosition().x + WIDTH_JUMP*0.5f - OFFSET_ACROBAT, samus->getPosition().y - HEIGHT_JUMP*0.5f);
		rect.left = position.x - (WIDTH_COLLISION)+1;
		rect.right = position.x - 1;
		rect.top = position.y + 1;
		rect.bottom = position.y + (JUMP_HEIGHT)-1;
	}
	else
	{
		VECTOR2 position(this->samus->getPosition().x - WIDTH_JUMP*0.5f + OFFSET_ACROBAT, samus->getPosition().y - HEIGHT_JUMP*0.5f);
		rect.left = position.x + 1;
		rect.right = position.x + (WIDTH_COLLISION)-1;
		rect.top = position.y + 1;
		rect.bottom = position.y + (JUMP_HEIGHT)-1;

	}
	samus->setBoundCollision(rect);
}
void SamusStateAcrobat::init()
{

	this->animation = samus->getJumpingAnim();
	this->samus->setOrigin(VECTOR2(0.5f, 0.5f));
	time = 0;
	this->position_to_jump = this->samus->getPosition().y;
	//this->samus->setVelocityY(-SAMUS_MAX_SPEED_Y);
	//velocity = -SAMUS_MAX_SPEED_Y;
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
		float distance = this->samus->getVelocity().y*dt*-1;
		jumpDistance += distance;
		time += dt;


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
		if(this->samus->getPosition().y >= position_to_jump)
		{
			this->samus->setStatus(eStatus::JUMPING);
		}
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
#pragma endregion

			//another object
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
			if (this->samus->isInDirection(eDirection::right))
			{
				this->samus->setPosition(VECTOR2(this->samus->getPosition().x + WIDTH_JUMP*0.5f - OFFSET_ACROBAT, this->samus->getPosition().y - HEIGHT_JUMP*0.5f));
			}
			else
			{
				this->samus->setPosition(VECTOR2(this->samus->getPosition().x - WIDTH_JUMP*0.5f + OFFSET_ACROBAT, this->samus->getPosition().y - HEIGHT_JUMP*0.5f));
			}
			break;
		case eStatus::STANDING:
			this->samus->setPositionY(positionCollide);
			if (this->samus->isInDirection(eDirection::right))
			{
				this->samus->setPosition(VECTOR2(this->samus->getPosition().x + WIDTH_JUMP*0.5f - OFFSET_ACROBAT +OFFSET_RUN,positionCollide));
			}
			else
			{
				this->samus->setPosition(VECTOR2(this->samus->getPosition().x - WIDTH_JUMP*0.5f + OFFSET_ACROBAT - OFFSET_RUN,positionCollide));
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



void SamusStateAcrobat::onStart()
{
	this->animation->start();
}

void SamusStateAcrobat::onExit()
{
	this->animation->stop();
	this->animation = nullptr;	
}



