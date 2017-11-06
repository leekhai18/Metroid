#include "Skree.h"


Skree::Skree(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::SKREE)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Skree");
	}

	animationRotate = new Animation(this->sprite, VECTOR2(0.5f, 0.5f), IndexManager::getInstance()->skreeYellow, NUM_FRAMES_SKREE, TIME_FRAME_DELAY);
	animationRotate->start();

	effectDeath = new SkreeEffectDeath(textureM, graphics);

	this->startPosition = VECTOR2(500, 100);
	this->target = VECTOR2ZERO;
	this->setPosition(this->startPosition);

	t = 0;
	timerDeath = 0;
}

Skree::Skree()
{
}


Skree::~Skree()
{
	delete this->effectDeath;
	this->effectDeath = nullptr;
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
			t += dt * RATE_BEZIER;
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
	delete this->sprite;
	delete this->animationRotate;

	this->sprite = nullptr;
	this->animationRotate = nullptr;
}

VECTOR2 Skree::getTarget()
{
	return this->target;
}

void Skree::setTarget(VECTOR2 target)
{
	if (this->target != target && abs(target.x - this->startPosition.x) < GAME_WIDTH*RATE_AREA_ACTIVE)
		this->target = target;
}
