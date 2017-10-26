#include "Samus.h"
#include "SamusStateManager.h"

Samus::Samus(TextureManager* textureM,Graphics* graphics, Input* input) : BaseObject(eID::SAMUS)
{
	this->input = input;
	this->sprite = new Sprite();
	if (! this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite character");
	}

	this->sprite->setPosition(GVector2(0, 0));
	this->setRect();

	startingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowStart, NUM_FRAMES_SAMUS_START, 1, false);
	runningAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRunningRight , NUM_FRAMES_SAMUS_RUNNING, 0.17f);
	rollingAnimation = new Animation(this->sprite, IndexManager::getInstance()->samusYellowRollingRight, NUM_FRAMES_SAMUS_ROLLING, 0.2f);

	SamusStateManager::getInstance()->init(this, input);
}

Samus::Samus()
{
	this->sprite = new Sprite();
}

Samus::~Samus()
{
	this->release();
}


void Samus::setRect()
{
	this->sprite->setSpriteDataRect(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowStart[3]].rect);
}

void Samus::draw()
{
	this->sprite->draw();
}

void Samus::handleInput(float dt)
{
	updateDirection();

	SamusStateManager::getInstance()->getCurrentState()->handleInput(dt);
}


void Samus::update(float dt)
{
	SamusStateManager::getInstance()->getCurrentState()->update(dt);
}

void Samus::release()
{
	input = nullptr;
	sprite = nullptr;
	runningAnimation = nullptr;
	rollingAnimation = nullptr;
	startingAnimation = nullptr;

	delete input, sprite, runningAnimation, rollingAnimation, startingAnimation;
}

void Samus::updateDirection()
{
	if (input->isKeyDown(VK_RIGHT))
	{
		this->setDirection(eDirection::right);
	}

	if (input->isKeyDown(VK_LEFT))
	{
		this->setDirection(eDirection::left);
	}
}

void Samus::running(float dt)
{

}

void Samus::turnUp()
{
}

void Samus::jump()
{
}

Animation * Samus::getStartingAnim()
{
	return this->startingAnimation;
}
