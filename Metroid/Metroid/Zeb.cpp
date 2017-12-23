#include "Zeb.h"

#define VELOCITY_X 120
#define VELOCITY_Y 120
#define TIME_FRAME_DELAY 0.2f
#define TIME_DELAY_BE_HIT 0.2f
#define TIME_RETURN_NOMAL 1.0f

Zeb::Zeb()
{
}

Zeb::Zeb(TextureManager * textureM, Graphics * graphics, EnemyColors color,Samus* samus) : BaseObject(eID::ZEB), IFreezable(IndexManager::getInstance()->zebBlue)
{
	this->samus = samus;
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zeb");
	}
	this->samusPosition = VECTOR2ZERO;
	initialize(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY);
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

	this->setOrigin(VECTOR2(0.5, 0.5));

	this->anim->start();
	isActivity = true;
	//canDraw = false;
}


Zeb::~Zeb()
{
	delete this->anim;
}
void Zeb::reInit()
{
	allowFly = false;
	isActivity = true;
	health = 2;
	this->setPosition(startPosition);
	canDraw = true;
	samusPosition = samus->getPosition();
}
void Zeb::handleVelocity(float dt)
{
	if (isActivity == true)
	{
		this->velocity.x = 0;

		this->anim->update(dt);

		if(!allowFly)
		{
			samusPosition = samus->getPosition();
		}
		if (this->getPosition().y + 50 <= this->samusPosition.y)
		{
			this->velocity.y = VELOCITY_Y;

			this->velocity.x = 0;

			direction = this->getPosition().x > this->samusPosition.x ? eDirection::left: eDirection::right ;

			allowFly = true;
			return;
		}
		else
		{
			
		}
		if (this->getPosition().y  >= this->samusPosition.y)
		{
			this->velocity.x = VELOCITY_X*direction;
			this->velocity.y = 0;
			//allowFly = true;
			
		}

	}

}

void Zeb::update(float dt)
{
	if (isActivity)
	{
		if (this->isCold)
		{
			timeReturnNormal += dt;
			if (timeReturnNormal >= TIME_RETURN_NOMAL)
			{
				this->anim->setPause(false);
				this->isCold = false;
			}
			else
			{
				this->sprite->setData(this->frameID[anim->getCurrentFrame()]);
				this->anim->setPause(true);
				return;
			}

		}

		if (beHit)
		{
			timerHit += dt;
			if (timerHit < TIME_DELAY_BE_HIT)
			{
				//this->animationRotate->setPause(true);
				this->setVelocity(VECTOR2(0, 0));
			}
			else
			{
				timerHit = 0;
				beHit = false;
				this->anim->setPause(false);

				if (this->health <= 0)
				{

					IExplosible::start();
					this->setVelocity(VECTOR2(0, 0));
					this->isActivity = false;
				}
			}
		}
		this->setPosition(VECTOR2(this->getPosition().x + velocity.x*dt,
			this->getPosition().y + velocity.y*dt));
	}
	if (this->getPosition().x < Camera::getInstance()->getBound().left - 20 || this->getPosition().x > Camera::getInstance()->getBound().right + 20)
	{
		reInit();
	}
}

void Zeb::draw()
{
	if (isActivity)
	{
		if (canDraw)
		{
			this->sprite->draw();
		}
	}
}
void Zeb::setBeHit(bool hit)
{
	this->beHit = hit;
}
void Zeb::decreaseHealth(float dame)
{
	this->health = this->health - dame;
}
void Zeb::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->getSprite()->getWidth() *0.5f;
	rect.right = position.x + this->getSprite()->getWidth() *0.5f;
	rect.top = position.y + this->getSprite()->getHeight() *0.5f;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f;

	this->boundCollision = rect;
}


void Zeb::setStartPosition(VECTOR2 startPosition)
{
	this->startPosition = startPosition;
}


void Zeb::setTarget(VECTOR2 samusPosition)
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
		//this->setScaleX(-1);
	}
}


