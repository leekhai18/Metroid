#include "Samus.h"
#include "SamusStateManager.h"
#include "Camera.h"
#include "GameDebug.h"


#define DISTANCE_MOVE_FRONT_GATE 20


Samus::Samus(TextureManager* textureM,Graphics* graphics, Input* input) : BaseObject(eID::SAMUS)
{
	this->input = input;
	this->sprite = new Sprite();
	if (! this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Samus");
	}

	this->setPosition(VECTOR2(640, 1267));
	runningNormalAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.05f);
	runningUpAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningUpRight, NUM_FRAMES_SAMUS_RUNNING, 0.05f);
	runningHittingRightAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowHittingAndRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.05f);
	rollingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRollingRight, NUM_FRAMES_SAMUS_ROLLING, 0.05f);
	jumpingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowJumpingRight, NUM_FRAMES_SAMUS_JUMPING, 0.04f);
	startingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowStart, NUM_FRAMES_SAMUS_START, 1, false);

	SamusStateManager::getInstance()->init(this, input);

	this->isFalling = false;
	this->acrobat = false;
	this->moveHorizontal = true;
	this->moveLeft = true;
	this->moveRight = true;
	this->jump = true;
	this->distance = 0;

	this->timerShoot = 0;
	bulletPool = new BulletPool(textureM, graphics, 20);
	this->listCollide = new list<CollisionReturn>();
}

Samus::Samus()
{
	this->sprite = new Sprite();
}

Samus::~Samus()
{
	this->release();
}

void Samus::draw()
{
	for (unsigned i = 0; i < this->bulletPool->getListUsing().size(); i++)
		this->bulletPool->getListUsing().at(i)->draw();

	this->sprite->draw();

}

void Samus::handleInput(float dt)
{
	if (!Camera::getInstance()->moveWhenSamusOnPort())
		SamusStateManager::getInstance()->getCurrentState()->handleInput(dt);

#pragma region handle camera
	if (!Camera::getInstance()->moveWhenSamusOnPort() && Camera::getInstance()->getNumPort() < 2)
	{
		if (Camera::getInstance()->canFolowOnLeft() || Camera::getInstance()->canFolowOnRight())
		{
			if ((input->isKeyUp(VK_LEFT) && input->isKeyUp(VK_RIGHT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT)) || this->isInStatus(eStatus::STANDING))
				Camera::getInstance()->setVelocity(VECTOR2(0, 0));
		}
	}
	Camera::getInstance()->setNumPort(0);

	//if (input->isKeyDown(VK_LEFT))
	//	Camera::getInstance()->setVelocity(VECTOR2(-150, 0));	
	//if (input->isKeyDown(VK_RIGHT))
	//	Camera::getInstance()->setVelocity(VECTOR2(150, 0));
	//if (input->isKeyDown(VK_UP))
	//	Camera::getInstance()->setVelocity(VECTOR2(0, 150));
	//if (input->isKeyDown(VK_DOWN))
	//	Camera::getInstance()->setVelocity(VECTOR2(0, -150));

#pragma endregion
}

void Samus::onCollision()
{
	SamusStateManager::getInstance()->getCurrentState()->onCollision();
	this->listCollide->clear();
}

void Samus::setIsCollidingPort(bool flag)
{
	this->isCollidingPort = flag;
}

bool Samus::isColliedPort()
{
	return isCollidingPort;
}

void Samus::setCanMoveToFrontGate(bool flag)
{
	this->moveToFontGate = flag;
}

void Samus::setListCanCollide(list<BaseObject*> list)
{
	this->listCanCollide = list;
}

void Samus::update(float dt)
{
	SamusStateManager::getInstance()->getCurrentState()->update(dt);

#pragma region handle camera
	bool isCameraMoving = false;
	if (!Camera::getInstance()->moveWhenSamusOnPort())
	{
		if (Camera::getInstance()->canFolowOnLeft())
		{
			if (this->getPosition().x < Camera::getInstance()->getActiveArea().left)
			{
				Camera::getInstance()->setVelocity(VECTOR2(this->getVelocity().x, 0));
				isCameraMoving = true;
			}
		}

		if (Camera::getInstance()->canFolowOnRight())
		{
			if (this->getPosition().x > Camera::getInstance()->getActiveArea().right)
			{
				Camera::getInstance()->setVelocity(VECTOR2(this->getVelocity().x, 0));
				isCameraMoving = true;
			}
		}

		if (this->isInStatus(eStatus::ACROBAT) || this->isInStatus(eStatus::JUMPING))
		{
			if (Camera::getInstance()->canFolowUp())
			{
				if (this->getPosition().y > Camera::getInstance()->getActiveArea().top)
				{
					Camera::getInstance()->setVelocity(VECTOR2(0, this->getVelocity().y));
					isCameraMoving = true;
				}
			}

			if (Camera::getInstance()->canFolowDown())
			{
				if (this->getPosition().y < Camera::getInstance()->getActiveArea().bottom)
				{
					Camera::getInstance()->setVelocity(VECTOR2(0, this->getVelocity().y));
					isCameraMoving = true;
				}
			}
		}

		if (!isCameraMoving)
			Camera::getInstance()->setVelocity(VECTOR2(0, 0));
	}

	if (isCollidingPort)
		this->setVelocityX(Camera::getInstance()->getVelocity().x);

	if (moveToFontGate && !Camera::getInstance()->moveWhenSamusOnPort())
	{
		float dis = dt * SAMUS_VERLOCITY_X;
		this->distance += dis;

		if (this->distance < DISTANCE_MOVE_FRONT_GATE)
			this->setPositionX(this->getPosition().x + dis*this->getDirection());
		else
		{
			this->distance = 0;
			moveToFontGate = false;
			this->setStatus(eStatus::STANDING);
		}
	}
#pragma endregion

	this->timerShoot += dt;
	for (unsigned i = 0; i < this->bulletPool->getListUsing().size(); i++)
		this->bulletPool->getListUsing().at(i)->update(dt);
}

void Samus::release()
{
	delete this->sprite;
	delete runningNormalAnimation;
	delete runningUpAnimation;
	delete runningHittingRightAnimation;
	delete rollingAnimation;
	delete startingAnimation;
	delete jumpingAnimation;
	delete bulletPool;
	this->listCollide->clear();
	delete this->listCollide;
}

void Samus::updateHorizontal(float dt)
{
	this->setPosition(this->getPosition().x + velocity.x*dt, this->getPosition().y);
}

void Samus::updateVertical(float dt)
{
	this->setPosition(this->getPosition().x, this->getPosition().y + velocity.y*dt);
}

bool Samus::isFaling()
{
	return this->isFalling;
}

bool Samus::isAcrobat()
{
	return acrobat;
}

bool Samus::canMoveLeft()
{
	return this->moveLeft;
}

bool Samus::canMoveRight()
{
	return this->moveRight;
}

bool Samus::isJump()
{
	return this->jump;
}

void Samus::setJump(bool jump)
{
	this->jump = jump;
}

void Samus::setCanMoveLeft(bool moveLeft)
{
	this->moveLeft = moveLeft;
}

void Samus::setCanMoveRight(bool moveRight)
{
	this->moveRight = moveRight;
}

void Samus::setFall(bool isFall)
{
	this->isFalling = isFall;
}
void Samus::setBoundCollision(MetroidRect rect)
{
	this->boundCollision = rect;
	GameDebug::getInstance()->setVertices(rect);
	this->setActiveBound();
}

void Samus::setActiveBound()
{
	// Can 1 con so hop ly
	this->activeBound.top = this->boundCollision.top + 50;
	this->activeBound.left = this->boundCollision.left - 50;
	this->activeBound.right = this->boundCollision.right + 50;
	this->activeBound.bottom = this->boundCollision.bottom - 50;
}

void Samus::setAcrobat(bool acrobat)
{
	this->acrobat = acrobat;
}

list<BaseObject*>& Samus::getListCanCollide()
{
	return this->listCanCollide;

}

Animation * Samus::getStartingAnim()
{
	return this->startingAnimation;
}

Animation * Samus::getRunningNormalAnim()
{
	return this->runningNormalAnimation;
}

Animation * Samus::getRunningUpAnim()
{
	return runningUpAnimation;
}

Animation * Samus::getRunningHittingRightAnim()
{
	return runningHittingRightAnimation;
}

Animation * Samus::getRollingAnim()
{
	return this->rollingAnimation;
}

Animation * Samus::getJumpingAnim()
{
	return this->jumpingAnimation;
}

list<CollisionReturn>* Samus::getListCollide()
{
	return this->listCollide;
}

