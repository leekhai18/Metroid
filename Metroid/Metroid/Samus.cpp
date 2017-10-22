#include "Samus.h"

Samus::Samus(TextureManager* textureM,Graphics* graphics, Input* input) : BaseObject(eID::SAMUS)
{
	this->input = input;
	this->sprite = new Sprite();

	if (! this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite character");
	}

	this->sprite->setPosition(GVector2(GAME_WIDTH / 2, GAME_HEIGHT / 2));
	this->setRect();

	currentAnimation = nullptr;
	runningAnimation = new Animation(this->sprite, SAMUS_BEHAVIOUR::running, 3, 0.2f);
	rollingAnimation = new Animation(this->sprite, SAMUS_BEHAVIOUR::rolling, 4, 0.2f);
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
	switch (this->status)
	{
		case eStatus::NORMAL:
		{
			this->sprite->setSpriteDataRect(SpriteManager::getInstance()->getSpritesData()[SAMUS_BEHAVIOUR::front].rect);
			break;
		}
		case eStatus::ENDING:
		{
			this->sprite->setSpriteDataRect(SpriteManager::getInstance()->getSpritesData()[SAMUS_BEHAVIOUR::ending].rect);
			break;
		}		
		case eStatus::JUMPING:
		{
			this->sprite->setSpriteDataRect(SpriteManager::getInstance()->getSpritesData()[SAMUS_BEHAVIOUR::jump].rect);
			break;
		}		
		default:
			break;
	}
}

void Samus::draw()
{
	this->sprite->draw();
}


void Samus::update(float dt)
{
	if (input->wasKeyPressed(VK_RIGHT))
	{
		this->setStatus(eStatus::RUNNING);

		if (currentAnimation != nullptr)
			currentAnimation->stop();

		this->sprite->flipHorizontal(false);
		currentAnimation = runningAnimation;
		currentAnimation->start();
	}
	if (input->wasKeyPressed(VK_LEFT))
	{
		this->setStatus(eStatus::RUNNING);

		if (currentAnimation != nullptr)
			currentAnimation->stop();

		this->sprite->flipHorizontal(true);
		currentAnimation = runningAnimation;
		currentAnimation->start();
	}
	if (input->wasKeyPressed(X_KEY))
	{
		this->setStatus(eStatus::JUMPING);
		if (currentAnimation != nullptr)
			currentAnimation->stop();

		this->setRect();
	}
	if (input->wasKeyPressed(VK_DOWN))
	{
		this->setStatus(eStatus::ROLLING);

		if (currentAnimation != nullptr)
			currentAnimation->stop();

		currentAnimation = rollingAnimation;
		currentAnimation->start();
	}

	if (currentAnimation != nullptr)
	{
		currentAnimation->update(dt);
	}
}

void Samus::release()
{
	input = nullptr;
	sprite = nullptr;
	currentAnimation = nullptr;
	runningAnimation = nullptr;
	rollingAnimation = nullptr;

	delete input, sprite, currentAnimation, runningAnimation, rollingAnimation;
}
