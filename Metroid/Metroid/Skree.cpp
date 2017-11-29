#include "Skree.h"
#define TIME_FRAME_DELAY 0.15f
#define X_OVER_HEAD 50
#define RATE_AREA_ACTIVE 0.15f
#define RATE_TIME_FRAME_UP 0.3f
#define EFFECT_DEATH_TIME 1.5f


Skree::Skree(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::SKREE)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Skree");
	}

	effectDeath = new SkreeEffectDeath(textureM, graphics);

	t = 0;
	timerDeath = 0;

	switch (color)
	{
	case Yellow:
		animationRotate = new Animation(this->sprite, IndexManager::getInstance()->skreeYellow, NUM_FRAMES_SKREE, TIME_FRAME_DELAY);
		health = 2;
		break;

	case Brown:
		animationRotate = new Animation(this->sprite, IndexManager::getInstance()->skreeBrown, NUM_FRAMES_SKREE, TIME_FRAME_DELAY);
		health = 2;
		break;

	default:
		break;
	}

	animationRotate->start();
}

Skree::Skree()
{
}


Skree::~Skree()
{
	delete this->effectDeath;
}

void Skree::update(float dt)
{
	if (this->animationRotate != nullptr)
		this->animationRotate->update(dt);

	if (this->target != VECTOR2ZERO && this->isInStatus(eStatus::START))
	{
		this->setStatus(eStatus::FALLING);
		this->animationRotate->setTimeFrameDelay(this->animationRotate->getTimeFrameDelay() * RATE_TIME_FRAME_UP);
	}

	if (this->isInStatus(eStatus::FALLING))
	{
		if (t < 1)
		{
			t += dt;
			this->setPosition((1-t)*(1-t)*startPosition + 2*(1-t)*t*VECTOR2(target.x, target.y - X_OVER_HEAD) + t*t*target);
		}
		else 
		{
			this->setStatus(eStatus::ENDING);
			this->effectDeath->init(this->getPosition());
		}
	}

	if (this->isInStatus(eStatus::ENDING))
	{
		if (timerDeath < EFFECT_DEATH_TIME)
			timerDeath += dt;
		else
		{
			if (this->animationRotate != nullptr)
				this->release();

			this->effectDeath->update(dt);
		}
	}
}

void Skree::draw()
{
		if (this->sprite != nullptr)
			this->sprite->draw();

		if (this->effectDeath->isInit())
			this->effectDeath->draw();
}


void Skree::release()
{
	delete this->animationRotate;
	delete this->sprite;

	this->sprite = nullptr;
	this->animationRotate = nullptr;
}

VECTOR2 Skree::getTarget()
{
	return this->target;
}

void Skree::setTarget(VECTOR2 target)
{
	if (this->target != target && abs(target.x - this->startPosition.x) < VIEWPORT_WIDTH*RATE_AREA_ACTIVE)
		this->target = target;
}

void Skree::initPositions(VECTOR2 stP)
{
	this->startPosition = stP;
	this->setPosition(stP);
	this->target = VECTOR2ZERO;
}
