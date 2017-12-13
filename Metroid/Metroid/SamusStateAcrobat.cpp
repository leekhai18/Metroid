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
			//set direction to left
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

				return;
			}
		}
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
	}
	else
	{
		this->samus->setVelocityY(this->samus->getVelocity().y + ACCELERATE_Y*dt);

	if(this->samus->getPosition().y <= position_to_jump)
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
				this->samus->setPositionY(addY - OFFSET_JUMP);
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



