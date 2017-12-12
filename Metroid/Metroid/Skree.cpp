#include "Skree.h"
#include "ObjectManager.h"
#include "Collision.h"
#include "Camera.h"

#define TIME_FRAME_DELAY 0.15f
#define AREA_ACTIVE_X 40
#define TIME_FRAME_DELAY_FALLING 0.05f
#define EFFECT_DEATH_TIME 0.5f
#define VELOCITY_X 80
#define VELOCITY_Y 130



Skree::Skree(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::SKREE)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Skree");
	}
	this->sprite->setOrigin(VECTOR2(0.5f, 0));

	effectDeath = new SkreeEffectDeath(textureM, graphics);

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

	target = VECTOR2ZERO;

	isActivity = true;
}

Skree::Skree()
{
}


Skree::~Skree()
{
	delete this->effectDeath;
	release();
}

void Skree::update(float dt)
{
	if (isActivity)
	{
		if (timerDeath < EFFECT_DEATH_TIME)
		{
			setBoundCollision();
			this->animationRotate->update(dt);
		}

		if (this->target != VECTOR2ZERO && this->isInStatus(eStatus::START)) // check init
		{
			this->setStatus(eStatus::FALLING);
			this->animationRotate->setTimeFrameDelay(TIME_FRAME_DELAY_FALLING);
			this->setVelocityY(-VELOCITY_Y);
		}

		if (this->isInStatus(eStatus::FALLING))
		{
			if (target.x > this->getPosition().x) // on right side
				this->setVelocityX(VELOCITY_X);
			else
				this->setVelocityX(-VELOCITY_X);

			if (this->getPosition().y > target.y) // check collision with wall
				this->setPosition(this->getPosition().x + this->getVelocity().x*dt, this->getPosition().y + this->getVelocity().y*dt);
			else
				this->setStatus(eStatus::ENDING);
		}

		if (this->isInStatus(eStatus::ENDING))
		{
			if (timerDeath < EFFECT_DEATH_TIME)
				timerDeath += dt;
			else
			{
				if (this->getPosition() != VECTOR2ZERO)
				{
					this->effectDeath->init(this->getPosition());
					BaseObject::setBoundCollision(MetroidRect(0, 0, 0, 0));
					this->setPosition(VECTOR2ZERO);
				}

				this->effectDeath->update(dt);

				if (this->effectDeath->isFinished())
					this->finish();
			}
		}
	}
	else
	{
		if (this->initPosition.x < Camera::getInstance()->getBound().left - 20 || this->initPosition.x > Camera::getInstance()->getBound().right + 20)
			reInit();
	}
}

void Skree::draw()
{
	if (isActivity)
	{
		if (this->getPosition() != VECTOR2ZERO)
			this->sprite->draw();

		this->effectDeath->draw();
	}
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
	if (isActivity)
	{
		if (!this->isInStatus(eStatus::ENDING))
		{
			if (abs(target.x - this->getPosition().x) < AREA_ACTIVE_X)
			{
				this->target = target;
			}
		}
	}
}

void Skree::setBoundCollision()
{
	boundCollision.left = this->getPosition().x - this->getSprite()->getWidth()*0.5f;
	boundCollision.top = this->getPosition().y;
	boundCollision.right = boundCollision.left + this->getSprite()->getWidth();
	boundCollision.bottom = boundCollision.top - this->getSprite()->getHeight();
}

void Skree::setInitPosition(VECTOR2 pos)
{
	this->initPosition = pos;
	this->setPosition(pos);
}

void Skree::reInit()
{
	isActivity = true;
	this->setPosition(initPosition);
	this->setStatus(eStatus::START);
	this->target = VECTOR2ZERO;
	timerDeath = 0;
	this->animationRotate->setTimeFrameDelay(TIME_FRAME_DELAY);
	this->velocity = VECTOR2ZERO;
}

void Skree::finish()
{
	isActivity = false;
	this->effectDeath->setPosition(VECTOR2ZERO);
}

void Skree::setActivity(bool flag)
{
	this->isActivity = flag;
}

bool Skree::isActivitied()
{
	return this->isActivity;
}
