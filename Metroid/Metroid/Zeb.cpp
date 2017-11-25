#include "Zeb.h"

#define VELOCITY_X 120
#define VELOCITY_Y 120
#define TIME_FRAME_DELAY 0.2f


Zeb::Zeb()
{
}

Zeb::Zeb(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::ZEB)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zeb");
	}
	this->samusPosition = VECTOR2ZERO;

	switch (color)
	{
	case Yellow:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zebYellow, NUM_FRAMES_ZEB, TIME_FRAME_DELAY);
		health = 1;
		break;

	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zebBrown, NUM_FRAMES_ZEB, TIME_FRAME_DELAY);
		health = 1;
		break;
	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zebRed, NUM_FRAMES_ZEB, TIME_FRAME_DELAY);
		health = 2;
		break;

	default:
		break;
	}

	this->anim->start();
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
