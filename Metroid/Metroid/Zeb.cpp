#include "Zeb.h"
#define VELOCITY_X 120
#define VELOCITY_Y 120
#define TIME_FRAME_DELAY 0.2f
#define TIME_DELAY_BE_HIT 0.2f
#define TIME_RETURN_NOMAL 1.0f

Zeb::Zeb()
{
}

Zeb::Zeb(TextureManager * textureM, Graphics * graphics,EnemyColors color,Samus* samus) : BaseObject(eID::ZEB), IFreezable(IndexManager::getInstance()->zebBlue)
{
	this->samus = samus;
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zeb");
	}
	this->samusPosition = VECTOR2ZERO;

	this->initExplosion(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY);
	this->initRocket(this->sprite, IndexManager::getInstance()->rocket, NUM_FRAMES_BONUS, TIME_FRAME_DELAY);
	this->initItem(this->sprite, IndexManager::getInstance()->bonusHealth, NUM_FRAMES_BONUS, TIME_FRAME_DELAY);

	switch (color)
	{
	case Yellow:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zebYellow, NUM_FRAMES_ZEB, TIME_FRAME_DELAY);
		reset = IndexManager::getInstance()->zebYellow[0];
		health = 2;
		break;

	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zebBrown, NUM_FRAMES_ZEB, TIME_FRAME_DELAY);
		reset = IndexManager::getInstance()->zebBrown[0];
		health = 2;
		break;
	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zebRed, NUM_FRAMES_ZEB, TIME_FRAME_DELAY);
		reset = IndexManager::getInstance()->zebRed[0];
		health = 2;
		break;

	default:
		break;
	}

	this->setOrigin(VECTOR2(0.5, 0.5));

	this->anim->start();
	isActivity = true;
	//canDraw = false;
	isHandle = true;
}


Zeb::~Zeb()
{
	delete this->anim;
}
bool Zeb::getHandle()
{
	return isHandle;
}
void Zeb::reInit()
{
	allowFly = false;
	isActivity = true;
	isHandle = true;
	health = 2;
	this->setPosition(startPosition);

	this->anim->setPause(false);
	canDraw = true;
	samusPosition = samus->getPosition();
	this->velocity = VECTOR2ZERO;
	IExplosible::reInit();
	IBonusable::reInit();
	this->isCold = false;
	sprite->setData(reset);
	canMoveHorizontal = false;
}
void Zeb::handleVelocity(float dt)
{
	if (isActivity && isHandle &&!isCold)
	{
		//this->velocity.x = 0;
		
		this->anim->update(dt);

		if(!allowFly)
		{
			samusPosition = samus->getPosition();
		}

		if (this->getPosition().y + 80 <= this->samusPosition.y)
		{
			this->velocity.y = VELOCITY_Y;

			this->velocity.x = 0;

			direction = this->getPosition().x > this->samusPosition.x ? eDirection::left: eDirection::right ;
			if (direction == eDirection::left)
			{
				this->sprite->setFlipX(false);
			}
			else
			{
				this->sprite->setFlipX(true);
			}


			allowFly = true; 
			canMoveHorizontal = true;
			return;
		}
		if (this->getPosition().y  >= this->samusPosition.y&&canMoveHorizontal )
		{
			this->velocity.x = VELOCITY_X*direction;
			this->velocity.y = 0;
			//canMoveHorizontal = false;
			//allowFly = true;
			
		}

	}

}

void Zeb::update(float dt)
{
	if (isActivity && isHandle )
	{
		if (this->isCold)
		{
			timeReturnNormal += dt;
			if (timeReturnNormal >= TIME_RETURN_NOMAL)
			{
				this->anim->setPause(false);
				this->isCold = false;
				timeReturnNormal = 0;
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
					//this->isActivity = false;
					isHandle = false;
				}
			}
		}
		this->setPosition(VECTOR2(this->getPosition().x + velocity.x*dt,
			this->getPosition().y + velocity.y*dt));
		setBoundCollision();
	}

	if (isExplose)
	{
		IBonusable::update(dt);
	}
	else
	{
		IExplosible::update(dt);
		if (isExplose)
		{
			IBonusable::start(samus->getRocket());
		}
	}

	if (!Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->boundCollision))
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
void Zeb::setStartBound(MetroidRect rect)
{
	this->startBound = rect;
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


