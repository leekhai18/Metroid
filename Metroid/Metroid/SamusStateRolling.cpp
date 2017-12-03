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
	if (this->samus->getDirection() == eDirection::right)
	{
		VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y);
		rect.left = position.x + 1;
		rect.right = position.x + (MAX_WIDTH)-1;
		rect.top = position.y - (ROLL_HEIGHT)+1;
		rect.bottom = position.y - 1;
	}
	else
	{
		VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y);
		rect.left = position.x - (MAX_WIDTH)+1;
		rect.right = position.x - 1;
		rect.top = position.y - (ROLL_HEIGHT)+1;
		rect.bottom = position.y - 1;

	}
	samus->setBoundCollision(rect);
}

void SamusStateRolling::init()
{
	this->animation = samus->getRollingAnim();
	if (this->samus->getDirection() == eDirection::right)
	{
		this->samus->setOrigin(VECTOR2(0, 1));
	}
	else
	{
		this->samus->setOrigin(VECTOR2(1, 1));
	}

}

void SamusStateRolling::handleInput(float dt)
{
	//handle press left button
	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			//When change side boundCollision not change
			MetroidRect bound = samus->getBoundCollision();
			//BoundCollision  Hand of samus
			//	  \			/
			//	   \ ___ __/	     ___
			//		|	|		    |	|
			//	1	|___|	-> 		|___| 2
			//		/			 
			//Pos__/__________ Orgin
			//	     ___	 /
			// 2    |	|	/
			//		|___|__/		
			//	
			//
			// Note: 1 right, 2 left
			//Position of samus 2 is on bottom right because we flipX samus 1 with orgin( 0, 1). 
			//And then we change to orgin(1,1) 
			//
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
			this->samus->setOrigin(VECTOR2(1.0f, 1.0f));
			//set flipX to true to turn left
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
			//When change side boundCollision not change
			MetroidRect bound = samus->getBoundCollision();
			//  Hand of samus   BoundCollision
			//	  	 /    _____/
			//	   _/___ /			 ___ _
			//	  |_|	|		    |	|_|
			//	1	|	|	-> 		|	|  2
			//		|___|			|___|
			//pos_______\_____orgin
			//	     ___ _	 /
			// 2    |	|_|	/
			//		|	|  /		
			//		|___| /	
			//       \___/
			// Note: 1 left, 2 right
			//when side of samus is right, set position.x = bound.left - 1
			this->samus->setPositionX(bound.left - 1);
			//change to orgin (0,1)
			this->samus->setOrigin(VECTOR2(0, 1.0f));
			//flipX change to false to turn right
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

	}
	// Handle horizontal
	if (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT))
	{
		//this->samus
		//this->samus->updateHorizontal(dt);
	}

	if (input->isKeyDown(VK_UP) || input->isKeyDown(VK_X))
	{
		float velocityY=(MAX_HEIHT);
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
				this->samus->setPositionY(i->positionCollision);
				this->samus->setVelocity(VECTOR2(0, 0));
				this->samus->setFall(false);
				setBoundCollision();
				return;
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

	if (this->samus->getStatus() != eStatus::ROLLING)
	{
		SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
	}
	this->samus->setCanMoveLeft(true);
	this->samus->setCanMoveRight(true);
	if (this->samus->isFaling())
	{
		this->samus->setVelocity(VECTOR2(0, SAMUS_VERLOCITY_Y));
	}
	else
	{
		this->samus->setVelocity(VECTOR2(0, 0));
	}

	setBoundCollision();
}


void SamusStateRolling::onStart()
{
	this->animation->start();
}

void SamusStateRolling::onExit()
{
	this->animation->stop();
}
