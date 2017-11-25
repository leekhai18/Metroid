#include "Zeb.h"

#define VELOCITY_X 120
#define VELOCITY_Y 120


Zeb::Zeb()
{
}

Zeb::Zeb(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ZEB)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zeb");
	}

	this->sprite->setOrigin(VECTOR2(0.5f, 0.5f));

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->zebYellow, NUM_FRAMES_ZEB, 0.2f);
	this->anim->start();

	this->setPosition(VECTOR2(GAME_WIDTH + this->sprite->getWidth()*0.5f, GAME_HEIGHT + this->sprite->getHeight()*0.5f));

	this->samusPosition = VECTOR2ZERO;
}


Zeb::~Zeb()
{
	delete this->anim;
}

void Zeb::update(float dt)
{	
	if (this->isInStatus(eStatus::RUNNING))
	{
		this->anim->update(dt);

		if (this->getPosition().y > this->samusPosition.y - 30) // Cao hon cai sung, van cham dau
		{
			this->setPositionY(this->getPosition().y - VELOCITY_Y*dt);
		}
		else
		{
			this->setPositionX(this->getPosition().x + VELOCITY_X*getDirection()*dt);

			// if position.x > areaActivity ---> delete
		}
	}
}

void Zeb::draw()
{
	this->sprite->draw();
}

void Zeb::init(VECTOR2 samusPosition)
{
	if (this->samusPosition == VECTOR2ZERO)
	{
		this->samusPosition = samusPosition;

		if (this->getPosition().x - samusPosition.x > 0)
		{
			this->setDirection(eDirection::left);
			this->setScaleX(1);
		}
		else
		{
			this->setDirection(eDirection::right);
			this->setScaleX(-1);
		}

		this->setStatus(eStatus::RUNNING);
	}
}
