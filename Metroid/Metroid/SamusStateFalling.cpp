#include "SamusStateFalling.h"
#include "SamusStateManager.h"


SamusStateFalling::SamusStateFalling()
{
}

SamusStateFalling::SamusStateFalling(Samus * samus, Input * input) : BaseState(samus, input)
{
}


SamusStateFalling::~SamusStateFalling()
{
}

void SamusStateFalling::init()
{
	this->samus->setFall(true);

	// Set Data for sprite
	const SpriteData data = SpriteManager::getInstance()->getSpritesData()[IndexManager::getInstance()->samusYellowJumpRight];
	this->samus->getSprite()->setSpriteDataRect(data.rect);
	this->samus->getSprite()->setSpriteWidth(data.width);
	this->samus->getSprite()->setSpriteHeigth(data.height);
}

void SamusStateFalling::handleInput(float dt)
{
	if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT))
	{
		this->samus->updateHorizontal(dt);
	}
}

void SamusStateFalling::update(float dt)
{
	this->samus->updateVertical(dt);

	this->handleInput(dt);

	// check if samus is on ground => you would set changetoState(Standing) by collision
	if (this->samus->getPositionY() >= GAME_HEIGHT*0.8) // just test
	{
		this->samus->setFall(false);
		this->samus->setStatus(eStatus::STANDING);
		SamusStateManager::getInstance()->changeStateTo(eStatus::STANDING);
	}
}

void SamusStateFalling::onStart()
{
}

void SamusStateFalling::onExit()
{
}
