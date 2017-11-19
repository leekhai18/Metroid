#include "Samus.h"
#include "SamusStateManager.h"

Samus::Samus(TextureManager* textureM,Graphics* graphics, Input* input) : BaseObject(eID::SAMUS)
{
	this->input = input;
	this->sprite = new Sprite();
	if (! this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Samus");
	}

	this->setPosition(VECTOR2(640, 3240));
	runningNormalAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.07f);
	runningUpAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningUpRight, NUM_FRAMES_SAMUS_RUNNING, 0.07f);
	runningHittingRightAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowHittingAndRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.07f);
	rollingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRollingRight, NUM_FRAMES_SAMUS_ROLLING, 0.07f);
	jumpingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowJumpingRight, NUM_FRAMES_SAMUS_JUMPING, 0.04f);
	startingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowStart, NUM_FRAMES_SAMUS_START, 1, false);

	SamusStateManager::getInstance()->init(this, input);

	this->isFalling = false;
	this->acrobat = false;
	this->moveHorizontal = true;
	this->totalHeightWasJumped = 0;
	this->moveLeft = true;
	this->moveRight = true;
	this->jump = true;

	this->timerShoot = 0;
	bulletPool = new BulletPool(textureM, graphics, 20);
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
	if (this->camera)
	{
		this->sprite->setTransformCamera(VECTOR2(GAME_WIDTH*0.5f - camera->getPosition().x, GAME_HEIGHT*0.5f - camera->getPosition().y));

		for (unsigned i = 0; i < this->bulletPool->getListUsing().size(); i++)
		{
			this->bulletPool->getListUsing().at(i)->getSprite()->setTransformCamera(VECTOR2(GAME_WIDTH*0.5f - camera->getPosition().x, GAME_HEIGHT*0.5f - camera->getPosition().y));
			this->bulletPool->getListUsing().at(i)->draw();
		}
	}

	this->sprite->draw();

}

void Samus::handleInput(float dt)
{
	SamusStateManager::getInstance()->getCurrentState()->handleInput(dt);
}

void Samus::onCollision(BaseObject * object, float dt)
{
	SamusStateManager::getInstance()->getCurrentState()->onCollision(object, dt);
}

void Samus::update(float dt)
{
	
	SamusStateManager::getInstance()->getCurrentState()->update(dt);


	this->timerShoot += dt;
	for (unsigned i = 0; i < this->bulletPool->getListUsing().size(); i++)
		this->bulletPool->getListUsing().at(i)->update(dt);
}

void Samus::release()
{
	delete sprite;
	delete runningNormalAnimation;
	delete runningUpAnimation;
	delete runningHittingRightAnimation;
	delete rollingAnimation;
	delete startingAnimation;
	delete jumpingAnimation;
	delete bulletPool;
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
	if (this->isFalling != isFall)
	{
		this->isFalling = isFall;
		
		if (this->isFalling == false)
			this->totalHeightWasJumped = 0;
	}
}

void Samus::setAcrobat(bool acrobat)
{
	this->acrobat = acrobat;
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

void Samus::setCamera(Camera * cam)
{
	this->camera = cam;
}

