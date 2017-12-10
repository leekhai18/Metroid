#include "Skree.h"
#include "ObjectManager.h"
#include "Collision.h"
#include "Camera.h"

#define TIME_FRAME_DELAY 0.15f
#define AREA_ACTIVE_X 40
#define TIME_FRAME_DELAY_FALLING 0.05f
#define EFFECT_DEATH_TIME 1.5f
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

	isActivity = false;
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
		// Set boundCollision
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
			if (this->getPosition().y > target.y) // check collision with wall
			{
				this->setPosition(this->getPosition().x + this->getVelocity().x*dt, this->getPosition().y + this->getVelocity().y*dt);
			}
			else
			{
				this->setStatus(eStatus::ENDING);
			}
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

				if (initPosition.x < Camera::getInstance()->getBound().left - 20 || initPosition.x > Camera::getInstance()->getBound().right + 20)
					reInit();
			}
		}
	}
}

void Skree::draw()
{
	if (isActivity)
	{
		this->sprite->draw();

		if (this->effectDeath->isInit())
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
			this->setVelocityX(0);

			if (this->target != target && abs(target.x - this->getPosition().x) < AREA_ACTIVE_X)
			{
				this->target = target;

				if (target.x > this->getPosition().x) // on right side
					this->setVelocityX(VELOCITY_X);
				else
					this->setVelocityX(-VELOCITY_X);
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
	isActivity = false;
	this->setPosition(initPosition);
	setBoundCollision();
	this->setStatus(eStatus::START);
	this->target = VECTOR2ZERO;
	timerDeath = 0;
	this->animationRotate->setTimeFrameDelay(TIME_FRAME_DELAY);
	this->effectDeath->setPosition(VECTOR2ZERO);
	this->velocity = VECTOR2ONE;
}

void Skree::setActivity(bool flag)
{
	this->isActivity = flag;
}

bool Skree::isActivitied()
{
	return this->isActivity;
}
