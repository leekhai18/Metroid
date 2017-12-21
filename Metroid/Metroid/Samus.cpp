#include "Samus.h"
#include "SamusStateManager.h"
#include "Camera.h"
#include "GameDebug.h"


#define DISTANCE_MOVE_FRONT_GATE 20
//Boss
//#define SAMUS_POS_X 808
//#define SAMUS_POS_Y 4214
//
//#define SAMUS_POS_X 640
//#define SAMUS_POS_Y 1267

//Boss
//#define SAMUS_POS_X 640
//#define SAMUS_POS_Y 4210

//MotherBrain
#define SAMUS_POS_X 640
#define SAMUS_POS_Y 3656
Samus::Samus(TextureManager* textureM,Graphics* graphics, Input* input) : BaseObject(eID::SAMUS)
{
	this->input = input;
	this->textureManager = textureM;
	this->graphics = graphics;

	this->sprite = new Sprite();
	if (! this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Samus");
	}

	this->setPosition(VECTOR2(SAMUS_POS_X, SAMUS_POS_Y));

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
	bulletPool = new BulletPool(textureM, graphics, 10);
	this->listCollide = new list<CollisionReturn>();

	boomPool = new BoomBombPool(textureM, graphics, 5);

	visible = true;
	this->numLives = 0;
	this->listNumLives = new list<Sprite*>();

	this->health = 30;
	this->healthIcon = new Sprite();
	if (!this->healthIcon->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Health Icon");
	}
	this->healthIcon->setData(IndexManager::getInstance()->enIcon);
	this->healthIcon->setPosition(VECTOR2(20, 20));
	this->healthIcon->setScale(VECTOR2(1.2f, 1.2f));

	this->healthText = new Text("30", eFont::body, 8, VECTOR2(45, 20), GraphicsNS::WHITE, false, true);
	this->healthText->initialize(graphics);
}

Samus::Samus()
{
}

Samus::~Samus()
{
	this->release();
	SamusStateManager::getInstance()->release();
}

void Samus::handleInput(float dt)
{
	
	SamusStateManager::getInstance()->getCurrentState()->handleInput(dt);

#pragma region handle camera
	if (!Camera::getInstance()->moveWhenSamusOnPort())
	{
		if (Camera::getInstance()->canFolowOnLeft() || Camera::getInstance()->canFolowOnRight())
		{
			if ((input->isKeyUp(VK_LEFT) && input->isKeyUp(VK_RIGHT)) || (input->isKeyDown(VK_LEFT) && input->isKeyDown(VK_RIGHT)) || this->isInStatus(eStatus::STANDING))
				Camera::getInstance()->setVelocity(VECTOR2(0, 0));
		}
	}

	//if (input->isKeyDown(VK_LEFT))
	//	Camera::getInstance()->setVelocity(VECTOR2(-150, 0));	
	//if (input->isKeyDown(VK_RIGHT))
	//	Camera::getInstance()->setVelocity(VECTOR2(150, 0));
	//if (input->isKeyDown(VK_UP))
	//	Camera::getInstance()->setVelocity(VECTOR2(0, 150));
	//if (input->isKeyDown(VK_DOWN))
	//	Camera::getInstance()->setVelocity(VECTOR2(0, -150));

	Camera::getInstance()->setNumPort(0);

#pragma endregion
}

void Samus::onCollision(float dt)
{
	SamusStateManager::getInstance()->getCurrentState()->onCollision(dt);
	this->listCollide->clear();
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

		if (Camera::getInstance()->canFolowUp() && this->getVelocity().y > 0)
		{
			if (this->getPosition().y > Camera::getInstance()->getActiveArea().top)
			{
				Camera::getInstance()->setVelocity(VECTOR2(0, this->getVelocity().y));
				isCameraMoving = true;
			}
		}

		if (Camera::getInstance()->canFolowDown() && this->getVelocity().y < 0)
		{
			if (this->getPosition().y < Camera::getInstance()->getActiveArea().bottom)
			{
				Camera::getInstance()->setVelocity(VECTOR2(0, this->getVelocity().y));
				isCameraMoving = true;
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
			this->setCanMoveLeft(true);
			this->setCanMoveRight(true);
		}
	}
#pragma endregion

	this->timerShoot += dt;
	for (unsigned i = 0; i < this->bulletPool->getListUsing().size(); i++)
		this->bulletPool->getListUsing().at(i)->update(dt);

	for (unsigned i = 0; i < this->boomPool->getListUsing().size(); i++)
		this->boomPool->getListUsing().at(i)->update(dt);
}

void Samus::draw()
{
	if (visible)
	{
		for (unsigned i = 0; i < this->bulletPool->getListUsing().size(); i++)
			this->bulletPool->getListUsing().at(i)->draw();

		for (unsigned i = 0; i < this->boomPool->getListUsing().size(); i++)
			this->boomPool->getListUsing().at(i)->draw();


		this->sprite->draw();
	}
}

void Samus::drawInFrontMap()
{
	for (list<Sprite*>::iterator i = listNumLives->begin(); i != listNumLives->end(); ++i)
	{
		(*i)->draw(false);
	}

	this->healthIcon->draw(false);
	
	this->healthText->setText(std::to_string(this->health));
	this->healthText->draw();
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

void Samus::release()
{
	delete this->sprite;
	delete runningNormalAnimation;
	delete runningUpAnimation;
	delete runningHittingRightAnimation;
	delete rollingAnimation;
	delete startingAnimation;
	delete jumpingAnimation;

	bulletPool->release();
	boomPool->release();

	this->listCollide->clear();
	delete this->listCollide;
	delete this->healthIcon;

	for (list<Sprite*>::iterator i = listNumLives->begin(); i != listNumLives->end(); ++i)
	{
		delete *i;
	}
	listNumLives->clear();
	delete listNumLives;
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

bool Samus::canJump()
{
	return this->jump;
}

void Samus::setCanJump(bool jump)
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
	//GameDebug::getInstance()->setVertices(rect);
	this->setActiveBound();
}

void Samus::setActiveBound()
{
	// Can 1 con so hop ly
	this->activeBound.top = this->boundCollision.top + 80;
	this->activeBound.left = this->boundCollision.left - 80;
	this->activeBound.right = this->boundCollision.right + 80;
	this->activeBound.bottom = this->boundCollision.bottom - 80;
}

void Samus::setVisible(bool visible)
{
	this->visible = visible;
}

bool Samus::getVisible()
{
	return this->visible;
}

void Samus::setHealth(int heal)
{
	this->health = heal;

	if (this->health <= 0)
	{
		if (this->numLives > 0)
		{
			this->numLives--;
			this->health = 99;
		}
		else
		{
			// GAME OVER
		}
	}
}

int Samus::getHealth()
{
	return this->health;
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

bool Samus::isHaveMariMaru()
{
	return isMariMaru;
}

void Samus::setMariMaru(bool flag)
{
	this->isMariMaru = flag;
}

int Samus::getNumLive()
{
	return this->numLives;
}

void Samus::setNumLive(int num)
{
	if (this->numLives < num) // ++
	{
		Sprite *live = new Sprite();
		if (!live->initialize(this->graphics, this->textureManager, SpriteManager::getInstance()))
		{
			throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite live");
		}
		live->setData(IndexManager::getInstance()->liveIcon);
		live->setPosition(VECTOR2(54 - this->numLives * live->getWidth() * 1.2f, 10));

		this->listNumLives->push_back(live);
	}
	else // --
	{
		this->listNumLives->pop_back();
	}

	this->numLives = num;
}

bool Samus::isHaveBomb()
{
	return isBomb;
}

void Samus::setBomb(bool flag)
{
	this->isBomb = flag;
}



