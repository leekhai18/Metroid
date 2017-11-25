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
	if (animation == NULL)
	{

		if (this->samus->getDirection() == eDirection::right)
		{
			VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y + HEIGHT_RUN);
			rect.left = position.x + 1;
			rect.right = position.x + (MAX_WIDTH)-1;
			rect.top = position.y - (MAX_HEIHT)+1;
			rect.bottom = position.y - 1;
		}
		else
		{
			VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y + HEIGHT_RUN);
			rect.left = position.x - (MAX_WIDTH)+1;
			rect.right = position.x - 1;
			rect.top = position.y - (MAX_HEIHT)+1;
			rect.bottom = position.y - 1;

		}
	}
	else
	{
		if (this->samus->getDirection() == eDirection::right)
		{
			VECTOR2 position(this->samus->getPosition().x - WIDTH_RUN*0.5f, samus->getPosition().y + HEIGHT_RUN*0.5f);
			rect.left = position.x + 1;
			rect.right = position.x + (MAX_WIDTH)-1;
			rect.top = position.y - (MAX_HEIHT)+1;
			rect.bottom = position.y - 1;
		}
		else
		{
			VECTOR2 position(this->samus->getPosition().x + WIDTH_RUN*0.5f, samus->getPosition().y + HEIGHT_RUN*0.5f);
			rect.left = position.x - (MAX_WIDTH)+1;
			rect.right = position.x - 1;
			rect.top = position.y - (MAX_HEIHT)+1;
			rect.bottom = position.y - 1;

		}
	}
	samus->setBoundCollision(rect);
}
void SamusStateJumping::init()
{
	//if(input->isKeyDown(VK_LEFT))
	if ((input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT)) && this->samus->isAcrobat())
	{
		this->animation = samus->getJumpingAnim();
		this->samus->setOrigin(VECTOR2(0.5f, 0.5f));
		return;
	}

	// Set Data for sprite
	if (samus->getDirection() == eDirection::left)
	{
		this->samus->setOrigin(VECTOR2(1.0f, 0));
	}
	else
	{
		this->samus->setOrigin(VECTOR2(0, 0));
	}
	this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowJumpRight);

}

void SamusStateJumping::handleInput(float dt)
{

#pragma region Horizontal
	if (input->isKeyDown(VK_RIGHT) && input->isKeyUp(VK_LEFT))
	{

		//set velocity to move right
		this->samus->setVelocityX(SAMUS_VERLOCITY_X);

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
				this->samus->setPositionX(bound.left - 1);
				//change to orgin (0,1)
				this->samus->setOrigin(VECTOR2(0, 0));
			}


			this->samus->setDirection(eDirection::right);
		}
	}

	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{

		//set velocity to move left
		this->samus->setVelocityX(-SAMUS_VERLOCITY_X);

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
				this->samus->setPositionX(bound.right + 1);
				//change orgin to (1,1)
				this->samus->setOrigin(VECTOR2(1.0f, 0));
				//set direction to left
				this->samus->setDirection(eDirection::left);
			}
			this->samus->setDirection(eDirection::left);
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

	//set velocity.y to jump up
	this->samus->setVelocityY(-SAMUS_VERLOCITY_Y);

	//if we release jump button, samus will jump to MIN_JUMP
	if (input->isKeyUp(VK_X))
	{
		if (jumpDistance >= MIN_JUMP)
		{
			this->samus->setFall(true);
		}
	}


	if (this->samus->isFaling() == false)
	{


		float distance = samus->getVelocity().y*dt;
		jumpDistance += distance*-1.0f;

		if (jumpDistance <= MAX_JUMP)
		{
			this->samus->setVelocityY(-SAMUS_VERLOCITY_Y);
			this->samus->setFall(false);
		}
		else
		{
			jumpDistance = 0;
			this->samus->setFall(true);
			this->samus->setVelocityY(SAMUS_VERLOCITY_Y);
		}
	}
	else
	{
		this->samus->setVelocityY(SAMUS_VERLOCITY_Y);
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
void SamusStateJumping::onCollision()
{
	list<CollisionReturn> dataReturn = Collision::getInstance()->getDataReturn();
	float addX = 0, addY = 0;
	for (list<CollisionReturn>::iterator object = dataReturn.begin(); object != dataReturn.end(); ++object)
	{
		eID objectID = (*object).idObject;
		CollideDirection collideDirection = (*object).direction;
		float entryTime = (*object).entryTime;
		float positionCollision = (*object).positionCollision;
		switch (objectID)
		{
			//handle collision with wall
		case eID::WALL:
			switch (collideDirection)
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
				positionCollide = positionCollision;
				this->samus->setStatus(eStatus::STANDING);
				break;
			case CollideDirection::BOTTOM:
				jumpDistance = 0;
				this->samus->setFall(true);
				this->samus->setVelocityY(0);
				addY = positionCollision;
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
void SamusStateJumping::update(float dt)
{
	if (this->animation != nullptr)
	{
		this->animation->update(dt);
	}
	if (this->samus->canMoveLeft() || this->samus->canMoveRight())
	{
		this->samus->updateHorizontal(dt);
	}

	this->samus->updateVertical(dt);


	setBoundCollision();
	if (this->samus->getStatus() != eStatus::JUMPING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::STANDING:
			this->samus->setVelocityX(0);
			if (animation == NULL)
			{
				this->samus->setPositionY(positionCollide);
			}
			else
			{
				if (this->samus->isInDirection(eDirection::right))
				{
					this->samus->setPosition(VECTOR2(this->samus->getPosition().x - WIDTH_RUN*0.5f, positionCollide));
				}
				else
				{
					this->samus->setPosition(VECTOR2(this->samus->getPosition().x + WIDTH_RUN*0.5f, positionCollide));
				}
			}
			break;
		default:
			break;
		}
		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
		return;
	}
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
	VECTOR2 tar;

	if (isUp)
	{
		if (this->samus->isInDirection(eDirection::right))
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.7f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);
		else if (this->samus->isInDirection(eDirection::left))
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.7f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);

		tar = VECTOR2(stP.x, stP.y - 100);
	}
	else
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.8f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			tar = VECTOR2(stP.x + 100, stP.y);
		}

		else if (this->samus->isInDirection(eDirection::left))
		{
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.8f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			tar = VECTOR2(stP.x - 100, stP.y);
		}
	}

	BulletPool::getInstance()->getBullet()->init(stP, tar);
}


