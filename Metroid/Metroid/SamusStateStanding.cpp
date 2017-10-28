#include "SamusStateStanding.h"
#include "SamusStateManager.h"


SamusStateStanding::SamusStateStanding()
{
}

SamusStateStanding::SamusStateStanding(Samus * samus, Input * input) : BaseState(samus, input)
{
	isPressed = false;
}

SamusStateStanding::~SamusStateStanding()
{
}

void SamusStateStanding::init()
{
	this->samus->getSprite()->setSpriteDataRect(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowTurnRight].rect);
}

void SamusStateStanding::handleInput(float dt)
{
	if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
	{
		this->samus->setStatus(eStatus::RUNNING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::RUNNING);
	}

	if (input->isKeyDown(VK_X))
	{
		this->samus->setStatus(eStatus::JUMPING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::JUMPING);
	}

	if (input->isKeyDown(VK_UP))
	{
		isPressed = true;
		this->samus->getSprite()->setSpriteDataRect(SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowTurnUp].rect);
	}

	if (isPressed && input->isKeyUp(VK_UP))
	{
		this->init();
	}

	if (input->isKeyDown(VK_DOWN))
	{
		this->samus->setStatus(eStatus::ROLLING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::ROLLING);
	}

	if (input->isKeyDown(VK_Z))
	{
		//Shoot
	}
}

void SamusStateStanding::update(float dt)
{
	this->handleInput(dt);
}

void SamusStateStanding::onStart()
{
}

void SamusStateStanding::onExit()
{
}
