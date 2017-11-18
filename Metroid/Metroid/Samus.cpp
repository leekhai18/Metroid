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

	runningNormalAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.07f);
	runningUpAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningUpRight, NUM_FRAMES_SAMUS_RUNNING, 0.07f);
	runningHittingRightAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowHittingAndRunningRight, NUM_FRAMES_SAMUS_RUNNING, 0.07f);
	rollingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRollingRight, NUM_FRAMES_SAMUS_ROLLING, 0.07f);
	jumpingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowJumpingRight, NUM_FRAMES_SAMUS_JUMPING, 0.04f);
	startingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowStart, NUM_FRAMES_SAMUS_START, 1, false);

	SamusStateManager::getInstance()->init(this, input);
	this->isFalling = false;
	this->totalHeightWasJumped = 0;

	this->setPosition(VECTOR2(100, GAME_HEIGHT*0.8));

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
		this->sprite->setTransformCamera(VECTOR2(GAME_WIDTH*0.5f - camera->getPosition().x, GAME_HEIGHT*0.5f - camera->getPosition().y));

	this->sprite->draw();

	for (unsigned i = 0; i < this->bulletPool->getListUsing().size(); i++)
		this->bulletPool->getListUsing().at(i)->draw();
}

void Samus::handleInput(float dt)
{
	SamusStateManager::getInstance()->getCurrentState()->handleInput(dt);
}


void Samus::update(float dt)
{
	this->handleInput(dt);
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
	this->setPosition(this->getPosition().x + SAMUS_VERLOCITY_X*dt*getDirection(), this->getPosition().y);
}

void Samus::updateVertical(float dt)
{
	if (this->isFalling)
	{
		this->setPosition(this->getPosition().x, this->getPosition().y + SAMUS_VERLOCITY_Y*dt);
	}
	else
	{
		float hps = SAMUS_VERLOCITY_Y*dt;
		totalHeightWasJumped += hps;

		if (totalHeightWasJumped <= MAX_HEIGHT_CAN_JUMP)
			this->setPosition(this->getPosition().x, this->getPosition().y + SAMUS_VERLOCITY_Y*dt*(-1));
		else
			this->setFall(true);
	}
}

bool Samus::isFaling()
{
	return this->isFalling;
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
