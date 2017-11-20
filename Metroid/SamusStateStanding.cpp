#include "SamusStateStanding.h"
#include "SamusStateManager.h"
#include "BulletPool.h"

#define TIME_TO_RUNNING 0.1f

SamusStateStanding::SamusStateStanding()
{
}

SamusStateStanding::SamusStateStanding(Samus * samus, Input * input) : BaseState(samus, input)
{
	isUp = false;
}

SamusStateStanding::~SamusStateStanding()
{
}

void SamusStateStanding::setBoundCollision()
{
	MetroidRect rect;
	if (this->samus->getDirection() == eDirection::right)
	{
		VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y);
		rect.left = position.x + 1;
		rect.right = position.x + (MAX_WIDTH)-1;
		rect.top = position.y - (MAX_HEIHT)+1;
		rect.bottom = position.y - 1;
	}
	else
	{
		VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y);
		rect.left = position.x - (MAX_WIDTH)+1;
		rect.right = position.x - 1;
		rect.top = position.y - (MAX_HEIHT)+1;
		rect.bottom = position.y - 1;

	}

	// set bounding
	samus->setBoundCollision(rect);
}

void SamusStateStanding::init()
{
	if (input->isKeyDown(VK_UP))
	{
		isUp = true;

		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnUp);
		this->samus->setOrigin(VECTOR2(0, 1.0f));

		return;
	}

	// Set Data for sprite
	this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
	if(samus->getDirection()==eDirection::left)
	{
		this->samus->setOrigin(VECTOR2(1.0f, 1.0f));
	}
	else
	{
		this->samus->setOrigin(VECTOR2(0, 1.0f));
	}
	
	timerToRunning = 0;
}

void SamusStateStanding::handleInput(float dt)
{
	setBoundCollision();

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
			this->samus->setPositionX(bound.right + 1);
			//change orgin to (1,1)
			this->samus->setOrigin(VECTOR2(1.0f, 1.0f));
			//set flipX to true to turn left
			this->samus->setFlipX(true);
			//set direction to left
			this->samus->setDirection(eDirection::left);
		}

		//check if moveLeft = false, it means samus is colliding with other object in right side
		if(this->samus->canMoveLeft() == true)
		{
			timerToRunning += dt;
			//when timeToRunning enough, we change to running state
			if (timerToRunning > TIME_TO_RUNNING)
			{
				//
				//
				//TOTAL_RECT	BoundCollision
				//	   \		  /
				//	 	\ __ ___ /
				//		 ||_|	|
				//		 |	|	|	
				//1		 |__|___|_ _ _ _position
				//			   /			|	
				//			  /				|
				//	Orgin	 /_ _ 		    |
				//	  __ ___	 /			|	
				// 2 ||_|	|	/			|
				//	 |	|	|  /		   /
				//	 |__|___| /		      /
				//       |__/			 /
				//       |_____________/  
				//when change to running state, orgin change to (0.5 , 0.5).       
				//We want boundCollision to be independent, so we use WIDTH_SAMUS and HEIGHT_SAMUS
				//(every sprite except rolling sprite will be inside this TOTAL_RECT which has width = WIDTH_SAMUS and height = HEIGHT_SAMUS) 
				this->samus->setPositionX(this->samus->getPosition().x - WIDTH_RUN*0.5f);
				//set direction to left
				this->samus->setDirection(eDirection::left);
				//change state to runnings
				this->samus->setStatus(eStatus::RUNNING);
				SamusStateManager::getInstance()->changeStateTo(eStatus::RUNNING);
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
			this->samus->setPositionX(bound.left - 1);
			//change to orgin (0,1)
			this->samus->setOrigin(VECTOR2(0, 1.0f));
			//flipX change to false to turn right
			this->samus->setFlipX(false);
			this->samus->setDirection(eDirection::right);

		}

		//check if canMoveRight = false, it means samus is colliding with other object in left side
		if(this->samus->canMoveRight() == true)
		{
			timerToRunning += dt;
			//when timeToRunning enough, we change to running state
			if (timerToRunning > TIME_TO_RUNNING)
			{
				//
				//
				//	BoundCollision    TOTAL_RECT
				//		   \		 /
				//	 	    \___ __ /
				//		  	|   |_||
				//			|	|  |
				//1			|___|__|     position
				//			|_ _ __ _ _ _ _ |
				//         /				|
				//	Orgin /__________		|
				//	     ______	    /		|	
				// 2    |	|_||   /		|
				//		|	|  |  /		   /
				//		|___|__| /		  /
				//        |_____/		 /
				//        |_ _ _ _ _ _ _/
				//when change to running state, orgin change to (0.5 , 1).
				//We want boundCollision to be independent, so we use WIDTH_SAMUS and HEIGHT_SAMUS
				//(every sprite except rolling sprite will be inside this TOTAL_RECT which has width = WIDTH_SAMUS and height = HEIGHT_SAMUS) 
				this->samus->setPositionX(this->samus->getPosition().x + WIDTH_RUN*0.5f); // update position for fixing origin
				//set direction to right
				this->samus->setDirection(eDirection::right);
				//change state to runnings
				this->samus->setStatus(eStatus::RUNNING);
				SamusStateManager::getInstance()->changeStateTo(eStatus::RUNNING);
				//we set jump to false, when we realease jump button it will change true
				this->samus->setJump(false);
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

	if (input->isKeyDown(VK_X)&&this->samus->isJump()== true)
	{
		this->samus->setFall(false);
		this->samus->setAcrobat(false);
		this->samus->setVelocityX(0);
		this->samus->setPositionY(this->samus->getPosition().y - HEIGHT_RUN);
		

		this->samus->setStatus(eStatus::JUMPING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::JUMPING);
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

	if (!isUp && input->isKeyUp(VK_Z) && this->samus->isInStatus(eStatus::HITTING))
	{
		// Set Data for sprite
		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowTurnRight);
		this->samus->setStatus(eStatus::STANDING);
	}

	if (input->isKeyDown(VK_DOWN))
	{
		this->samus->setStatus(eStatus::ROLLING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::ROLLING);
		return;
	}

	if (input->isKeyDown(VK_Z))
	{
		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
			this->samus->setStatus(eStatus::HITTING);

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
		}
	}
}

void SamusStateStanding::update(float dt)
{
}

void SamusStateStanding::onStart()
{
}

void SamusStateStanding::onExit()
{
}

void SamusStateStanding::fire()
{
	VECTOR2 stP;
	VECTOR2 tar;

	if (isUp)
	{
		if (this->samus->isInDirection(eDirection::right))
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.65f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);
		else if (this->samus->isInDirection(eDirection::left))
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.65f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.9f);

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
