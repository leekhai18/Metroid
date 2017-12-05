#include "SamusStateStanding.h"
#include "SamusStateManager.h"
#include "BulletPool.h"
#include "GameLog.h"

#define TIME_TO_RUNNING 0.1f

SamusStateStanding::SamusStateStanding()
{
}
void SamusStateStanding::setBoundCollision()
{
	MetroidRect rect;
	if (this->samus->getDirection() == eDirection::right)
	{
		VECTOR2 position(this->samus->getPosition().x - OFFSET_RUN, samus->getPosition().y);
		rect.left = position.x - (WIDTH_COLLISION) + 1;
		rect.right = position.x - 1;
		rect.top = position.y - (MAX_HEIHT)+1;
		rect.bottom = position.y - 1;

	}
	else
	{
		VECTOR2 position(this->samus->getPosition().x + OFFSET_RUN, samus->getPosition().y);
		rect.left = position.x + 1;
		rect.right = position.x + (WIDTH_COLLISION)-1;
		rect.top = position.y - (MAX_HEIHT)+1;
		rect.bottom = position.y - 1;

	}
	samus->setBoundCollision(rect);
}
SamusStateStanding::SamusStateStanding(Samus * samus, Input * input) : BaseState(samus, input)
{
	isUp = false;
}

SamusStateStanding::~SamusStateStanding()
{
}

void SamusStateStanding::init()
{
	if (samus->getDirection() == eDirection::left)
	{
		this->samus->setOrigin(VECTOR2(0, 1));
	}
	else
	{
		this->samus->setOrigin(VECTOR2(1, 1));
	}

	if (input->isKeyDown(VK_UP))
	{
		isUp = true;

		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnUp);

		return;
	}
	// Set Data for sprite
	this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
	setBoundCollision();
	timerToRunning = 0;
}

void SamusStateStanding::handleInput(float dt)
{
	if (input->isKeyUp(VK_LEFT) && input->isKeyUp(VK_RIGHT))
		timerToRunning = 0;

	//handle press left button
	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			//When change side boundCollision not change
			MetroidRect bound = samus->getBoundCollision();
			//BoundCollision  Hand of samus
			//	  \			/
			//	   \ ___ _ /	   _ ___
			//		|	|_|		  |_|	|
			//	1	|	|	-> 		|	| 2
			//		|___|			|___|
			//Pos__/__________ Orgin
			//	   _ ___	 /
			// 2  |_|	|	/
			//		|	|  /		
			//		|___|_/	
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
			this->samus->setPositionX(bound.left - 1 - OFFSET_RUN);
			//change orgin to (1,1)
			this->samus->setOrigin(VECTOR2(0, 1));
			//set flipX to true to turn left
			this->samus->setFlipX(true);
			//set direction to left
			this->samus->setDirection(eDirection::left);
		}

		//check if moveLeft = false, it means samus is colliding with other object in right side
		if (this->samus->canMoveLeft() == true)
		{
			timerToRunning += dt;
			//when timeToRunning enough, we change to running state
			if (timerToRunning > TIME_TO_RUNNING)
			{

				//set direction to left
				this->samus->setDirection(eDirection::left);
				//change state to runnings
				this->samus->setStatus(eStatus::RUNNING);
				//we set jump to false, when we realease jump button it will change true
				this->samus->setJump(false);
				//reset timer
				timerToRunning = 0;
				return;
			}
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
			this->samus->setPositionX(bound.right + 1 + OFFSET_RUN);
			//change to orgin (0,1)
			this->samus->setOrigin(VECTOR2(1, 1));
			//flipX change to false to turn right
			this->samus->setFlipX(false);
			this->samus->setDirection(eDirection::right);

		}

		//check if moveRight = false, it means samus is colliding with other object in left side
		if (this->samus->canMoveRight() == true)
		{
			timerToRunning += dt;
			//when timeToRunning enough, we change to running state
			if (timerToRunning > TIME_TO_RUNNING)
			{

				//set direction to right
				this->samus->setDirection(eDirection::right);
				//we set jump to false, when we realease jump button it will change true
				this->samus->setJump(false);
				//change state to runnings
				this->samus->setStatus(eStatus::RUNNING);
				//reset timer
				timerToRunning = 0;
				return;
			}
		}
	}

	//reset jump when user release jump button
	if (input->isKeyUp(VK_X))
	{
		this->samus->setJump(true);
	}

	if (input->isKeyDown(VK_X) && this->samus->isJump() == true)
	{
		this->samus->setFall(false);
		this->samus->setAcrobat(false);
		this->samus->setVelocityX(0);
		this->samus->setStatus(eStatus::JUMPING);
		return;
	}

	if (input->isKeyDown(VK_UP))
	{
		isUp = true;

		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnUp);
	}

	if (isUp && input->isKeyUp(VK_UP))
	{
		isUp = false;

		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
	}

	if (input->isKeyDown(VK_DOWN))
	{
		this->samus->setVelocityX(0);
		this->samus->setStatus(eStatus::ROLLING);
		return;
	}

	if (!isUp && input->isKeyUp(VK_Z) && isShoot)
	{
		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
		isShoot = false;
	}

	if (input->isKeyDown(VK_Z))
	{
		// Set up sprite Shooting
		if (isUp)
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingUp);
		}
		else
		{
			// Set Data for sprite
			this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingRight);
		}

		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
			this->isShoot = true;
		}
	}
}

void SamusStateStanding::update(float dt)
{

	setBoundCollision();
	if (this->samus->getStatus() != eStatus::STANDING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::JUMPING:
			//change orgin.y = 1 in running state to orgin.y = 0 in jumping state, so we change position.y		

			this->samus->setPositionY(this->samus->getPosition().y + HEIGHT_RUN);

			//we add 
			if (this->samus->isInDirection(eDirection::right))
			{
				this->samus->setPositionX(this->samus->getPosition().x - OFFSET_RUN);
			}
			else
			{
				this->samus->setPositionX(this->samus->getPosition().x + OFFSET_RUN);
			}
			//this->samus->setPositionX(this->samus->getPosition().x )
			break;
		case eStatus::RUNNING:
			//MetroidRect bound = samus->getBoundCollision();
			if (this->samus->isInDirection(eDirection::right))
			{
				//	BoundCollision    
				//		   \		 
				//	 	    \_ _ _ 
				//		  	|   |_|
				//			|	| 
				//1			|___|        position
				//			|_ _ __ _ _ _ _ |
				//         /				|
				//	Orgin /_ _ _ _ _ 		|
				//	     _ _ _	    /		|	
				// 2    |	|_|    /		|
				//		|	|     /		   /
				//		|___|    /		  /
				//          |___/		 /
				//          |_ _ _ _ _ _/
				//(1) when change to running state, orgin change to (1 , 1), and position move to right side of collision.
				//Because to advoid deviating of samus's head. In sprites of run samus, samus's head is in right side.
				//In other hand, head in sprite of stand samus is in right side of boundCollision not right side of sprite.
				//So first we add position.x with width of stand (WIDTH_STAND) and then add it with offset of collison in x axis
				//(offset of collision x axis(OFFSET_COLLISION_X) is to move bound collision to right with its length to see like game)
				//The last, we subtract with offset run sprite to prevent head of samus in standing state to running state from 
				//deviating 
				this->samus->setPositionX(this->samus->getPosition().x - OFFSET_RUN);
			}
			else
			{
				//       	BoundCollision
				//	     		  /
				//	       _ _ _ /
				//		  |_|	|
				//		 	|	|	
				//1		    |___|_ _ _ _position
				//			   /			|	
				//			  /				|
				//	Orgin_ _ /_ _ 		    |
				//	   _ _ _ 	 /			|	
				// 2  |_|	|	/			|
				//	 	|	|  /		   /
				//	    |___| /		      /
				//      |_ _ /			 /
				//      |_ _ _ __ _ _ _ /  
				//The same direction (1)
				this->samus->setPositionX(this->samus->getPosition().x + OFFSET_RUN);
			}
			break;
		case eStatus::ROLLING:
			break;
		default:
			break;
		}
		SamusStateManager::getInstance()->changeStateTo(this->samus->getStatus());
	}
}



void SamusStateStanding::onStart()
{
}

void SamusStateStanding::onExit()
{

}

void SamusStateStanding::onCollision()
{
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
		case eID::SKREE:
			GAMELOG("VA CHAM SKREE");
			break;

		default:
			break;
		}
	}
}

void SamusStateStanding::fire()
{
	VECTOR2 stP;
	Bullet* bullet = BulletPool::getInstance()->getBullet();


	if (isUp)
	{
		if (this->samus->isInDirection(eDirection::right))
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.65f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);
		else if (this->samus->isInDirection(eDirection::left))
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.65f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);

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
