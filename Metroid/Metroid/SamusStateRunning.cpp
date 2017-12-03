#include "SamusStateRunning.h"
#include "SamusStateManager.h"
#include "GameLog.h"
#include "Camera.h"
#include "GateBlue.h"

SamusStateRunning::SamusStateRunning()
{
}

SamusStateRunning::SamusStateRunning(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateRunning::~SamusStateRunning()
{

}
void SamusStateRunning::setBoundCollision()
{
	MetroidRect rect;
	//example of right side, left side offset is the same, you can image from right side
	//	BoundCollision  WIDTH = 20  
	//	  	\_ _          / 
	//	 	   _|_ _ _ __/ 
	//		  | |   |_|__|
	//		  | |	| |  |
	//        |_|___|_|__|      
	// _ _ _ _ _      |_ _position  
	//|       |_|	   __           
	//|	              |__|_ _offset_run        
	//|                               
	//offset_collison_x              
	//(WIDTH_COLLISION -2) because bound collision will be subtract to 1 in four side(top,left,right,bottom)
	//

	if (this->samus->getDirection() == eDirection::right)
	{

		VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y);
		rect.left = position.x - (WIDTH_COLLISION)+1;
		rect.right = position.x - 1;
		rect.top = position.y - (MAX_HEIHT)+1;
		rect.bottom = position.y - 1;

	}
	else
	{
		VECTOR2 position(this->samus->getPosition().x, samus->getPosition().y);
		rect.left = position.x + 1;
		rect.right = position.x + (WIDTH_COLLISION)-1;
		rect.top = position.y - (MAX_HEIHT)+1;
		rect.bottom = position.y - 1;

	}
	samus->setBoundCollision(rect);
}

void SamusStateRunning::init()
{
	runningNormal = samus->getRunningNormalAnim();
	runningUp = samus->getRunningUpAnim();
	runningHittingRight = samus->getRunningHittingRightAnim();

	if (this->samus->isInDirection(eDirection::right))
		this->samus->setOrigin(VECTOR2(1, 1));
	else
		this->samus->setOrigin(VECTOR2(0, 1));
	
	this->animation = runningNormal;
	this->animation->setValueOfCurrentFrame(0);
	setBoundCollision();
}

void SamusStateRunning::handleInput(float dt)
{
#pragma region Horizontal
	if ((input->isKeyUp(VK_RIGHT) && input->isKeyUp(VK_LEFT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT))
		|| (input->isKeyDown(VK_RIGHT) && this->samus->isInDirection(eDirection::left))
		|| (input->isKeyDown(VK_LEFT) && this->samus->isInDirection(eDirection::right)))
	{
		//reset velocity
		this->samus->setVelocityX(0);
		this->samus->setStatus(eStatus::STANDING);
		return;
	}
	if (input->isKeyDown(VK_LEFT) && input->isKeyUp(VK_RIGHT))
	{
		this->samus->setVelocityX(-SAMUS_VERLOCITY_X);
	}

	if (input->isKeyUp(VK_LEFT) && input->isKeyDown(VK_RIGHT))
	{
		this->samus->setVelocityX(SAMUS_VERLOCITY_X);
	}


#pragma endregion

	if (input->isKeyUp(VK_X) == true)
	{
		this->samus->setJump(true);
	}
	//When we press jump key, change state to jump
	if (input->isKeyDown(VK_X) && this->samus->isJump() == true)
	{
		//reset velocity
		this->samus->setVelocityX(0);
		this->samus->setFall(false);
		this->samus->setAcrobat(true);
		this->samus->setStatus(eStatus::JUMPING);
		return;
	}

	//when we press up button hand of samus will raise to sky
	if (input->isKeyDown(VK_UP))
	{
		//stop current animation
		this->animation->stop();
		//change animation to hand raise to sky
		this->animation = runningUp;
		//start new animation
		this->animation->start();
	}
	//when we press shot button and not press up button, samus will shot horizontal 
	if (input->isKeyDown(VK_Z) && input->isKeyUp(VK_UP))
	{
		//stop current animation
		this->animation->stop();
		//change animation to shot horizontal
		this->animation = runningHittingRight;
		//start new animation
		this->animation->start();

		//we must press shot button enoung long to samus shot
		this->isUp = false;
		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
		}
	}

	if (input->isKeyDown(VK_UP) && input->isKeyDown(VK_Z))
	{
		this->animation->stop();

		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingUp);
		this->animation = runningUp;
		this->animation->start();

		this->isUp = true;
		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
		}
	}

	if (input->isKeyUp(VK_UP) && input->isKeyUp(VK_Z) && (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT)))
	{
		this->animation->stop();

		this->animation = runningNormal;
		this->animation->start();
	}

	if (!Collision::getInstance()->checkOnGround(samus->getBoundCollision()))
	{
		this->samus->setAcrobat(false);
		this->samus->setFall(true);
		this->samus->setVelocityX(0);

		this->samus->setStatus(eStatus::JUMPING);
		return;
	}

	if (input->isKeyUp(VK_X) == true)
	{
		this->samus->setJump(true);
	}
	//When we press jump key, change state to jump
	if (input->isKeyDown(VK_X) && this->samus->isJump() == true)
	{
		//reset velocity
		this->samus->setVelocityX(0);
		this->samus->setFall(false);
		this->samus->setAcrobat(true);
		this->samus->setStatus(eStatus::JUMPING);
		return;
	}

	//when we press up button hand of samus will raise to sky
	if (input->isKeyDown(VK_UP))
	{
		//stop current animation
		this->animation->stop();
		//change animation to hand raise to sky
		this->animation = runningUp;
		//start new animation
		this->animation->start();
	}
	//when we press shot button and not press up button, samus will shot horizontal 
	if (input->isKeyDown(VK_Z) && input->isKeyUp(VK_UP))
	{
		//stop current animation
		this->animation->stop();
		//change animation to shot horizontal
		this->animation = runningHittingRight;
		//start new animation
		this->animation->start();

		//we must press shot button enoung long to samus shot
		this->isUp = false;
		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
		}
	}

	if (input->isKeyDown(VK_UP) && input->isKeyDown(VK_Z))
	{
		this->animation->stop();

		this->samus->getSprite()->setData(IndexManager::getInstance()->samusYellowHittingUp);
		this->animation = runningUp;
		this->animation->start();

		this->isUp = true;
		if (this->samus->timerShoot > TIME_SHOOTING)
		{
			this->fire();
			this->samus->timerShoot = 0;
		}
	}

	if (input->isKeyUp(VK_UP) && input->isKeyUp(VK_Z) && (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT)))
	{
		this->animation->stop();

		this->animation = runningNormal;
		this->animation->start();
	}

	if (!Collision::getInstance()->checkOnGround(samus->getBoundCollision()))
	{
		this->samus->setAcrobat(false);
		this->samus->setFall(true);
		this->samus->setVelocityX(0);

		this->samus->setStatus(eStatus::JUMPING);
		return;
	}

}
void SamusStateRunning::onCollision()
{
	for (auto i = this->samus->getListCollide()->begin(); i != this->samus->getListCollide()->end(); i++)
	{
		switch (i->object->getId())
		{
#pragma region Wall
		case eID::WALL:
			switch (i->direction)
			{
			case CollideDirection::LEFT:
				this->samus->setVelocityX(0);
				this->samus->setCanMoveRight(false);
				this->samus->setStatus(eStatus::STANDING);
				break;
			case CollideDirection::RIGHT:
				this->samus->setVelocityX(0);
				this->samus->setCanMoveLeft(false);
				this->samus->setStatus(eStatus::STANDING);
				break;
			case CollideDirection::TOP:
				break;
			}
			break;

		case eID::PORT:
			switch (i->direction)
			{
			case LEFT:
				Camera::getInstance()->setVelocity(VECTOR2(SAMUS_VERLOCITY_X, 0));
				Camera::getInstance()->setOnPort(true);
				this->animation->setPause(true);
				this->samus->setIsCollidingPort(true);
				break;
			case RIGHT:
				Camera::getInstance()->setVelocity(VECTOR2(-SAMUS_VERLOCITY_X, 0));
				Camera::getInstance()->setOnPort(true);
				this->animation->setPause(true);
				this->samus->setIsCollidingPort(true);
				break;
			default:
				break;
			}
			break;
#pragma endregion

#pragma region GATE
		case eID::GATEBLUE:
			switch (i->direction)
			{
			case CollideDirection::LEFT:
			{
				if (Camera::getInstance()->moveWhenSamusOnPort())
				{
					GateBlue* gate = static_cast<GateBlue*>(i->object);
					gate->setIsCollideSamusInPort(true);

					this->samus->setIsCollidingPort(false);
					this->samus->setVelocityX(0);
					this->samus->setCanMoveToFrontGate(true);
				}
				else
				{
					this->samus->setVelocityX(0);
					this->samus->setCanMoveRight(false);
					this->samus->setStatus(eStatus::STANDING);
				}

				break;
			}
			case CollideDirection::RIGHT:
			{
				if (Camera::getInstance()->moveWhenSamusOnPort())
				{
					GateBlue* gate = static_cast<GateBlue*>(i->object);
					gate->setIsCollideSamusInPort(true);

					this->samus->setIsCollidingPort(false);
					this->samus->setVelocityX(0);
					this->samus->setCanMoveToFrontGate(true);
				}
				else
				{
					this->samus->setVelocityX(0);
					this->samus->setCanMoveLeft(false);
					this->samus->setStatus(eStatus::STANDING);
				}

				break;
			}
			}
			break;
#pragma endregion

#pragma region SKREE
		case eID::SKREE:
			GAMELOG("VA CHAM SKREE");
			break;

#pragma endregion
		default:
			break;
		}

	}
}
void SamusStateRunning::update(float dt)
{
	this->animation->update(dt);

	samus->updateHorizontal(dt);

	setBoundCollision();

	if (this->samus->getStatus() != eStatus::RUNNING)
	{
		switch (this->samus->getStatus())
		{
		case eStatus::JUMPING:

			//stop current animation
			this->animation->stop();
			//change orgin.y = 1 in running state to orgin.y = 0 in jumping state, so we change position.y
			this->samus->setPosition(this->samus->getPosition().x, this->samus->getPosition().y - HEIGHT_RUN);
			break;
		case eStatus::STANDING:
			//When change to state standing, we must change position because orgin of state running is (0.5 , 1) 
			//and orgin of state running is (0,1) or (1,1) depend on samus side 
			if (samus->getDirection() == eDirection::right)
			{
				//	BoundCollision    TOTAL_RECT
				//		   \		 /
				//	 	    \___ __ /
				//		  	|   |_||
				//			|	|  |
				//1			|___|__|     position
				//			   |_ _ _ _ _ _ |	
				//         _ _/				|
				//	Orgin /_ _ _ _ _		|
				//	     ______	    / 		|	
				// 2    |	|_||   /		|
				//		|	|  |  /		   /
				//		|___|__| /		  /
				//      |_ _ _ _/		 /
				//        |_ _ _ _ _ _ _/
				//when change to standing state, orgin change from (0.5, 1) to (0 , 1).  
				this->samus->setPositionX(this->samus->getPosition().x + OFFSET_RUN);
			}
			else
			{
				//
				//
				//TOTAL_RECT	BoundCollision
				//	   \		  /
				//	 	\ __ ___ /
				//		 ||_|	|
				//		 |	|	|	
				//1		 |__|___|        position
				//			/_ _ _ _ _ _ _ _|	
				//         /				|
				//	Orgin /_ _ _ _			|
				//	  __ ___	 /			|	
				// 2 ||_|	|	/			|
				//	 |	|	|  /		   /
				//	 |__|___| /		      /
				//   |_ _ _ |/			 /
				//          | _ _ _ _ _ /  
				//when change to standing state, orgin change from (0.5, 1) to (1 , 1). 
				this->samus->setPositionX(this->samus->getPosition().x - OFFSET_RUN);
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



void SamusStateRunning::onStart()
{
	runningNormal->setPause(false);
	runningUp->setPause(false);
	runningHittingRight->setPause(false);

	this->animation->start();
}

void SamusStateRunning::onExit()
{
	this->animation->stop();
}

void SamusStateRunning::fire()
{
	VECTOR2 stP;
	Bullet* bullet = BulletPool::getInstance()->getBullet();

	if (isUp)
	{
		if (this->samus->isInDirection(eDirection::right))
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.3f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.8f);
		else if (this->samus->isInDirection(eDirection::left))
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.3f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.8f);

		bullet->setVelocity(VECTOR2(0, -VELOCITY));
	}
	else
	{
		if (this->samus->isInDirection(eDirection::right))
		{
			stP = VECTOR2(this->samus->getPosition().x + this->samus->getSprite()->getWidth()*0.4f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			bullet->setVelocity(VECTOR2(VELOCITY, 0));
		}

		else if (this->samus->isInDirection(eDirection::left))
		{
			stP = VECTOR2(this->samus->getPosition().x - this->samus->getSprite()->getWidth()*0.4f, this->samus->getPosition().y - this->samus->getSprite()->getHeight()*0.7f);
			bullet->setVelocity(VECTOR2(-VELOCITY, 0));
		}
	}

	bullet->init(stP);
}
