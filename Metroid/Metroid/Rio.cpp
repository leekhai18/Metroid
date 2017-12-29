#include "Rio.h"
#include "Camera.h"
#include "Collision.h"
#define RATE_BEZIER 0.45f
#define TIME_FRAME_DELAY 0.15f
#define WIDTH_AREA_ACTIVE 100
#define HEIGHT_AREA_ACTIVE 130
#define VELOCITY_X 60
#define TIME_DELAY_BE_HIT 0.2f
#define TIME_RETURN_NOMAL 5.0f
Rio::Rio(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::RIO), IFreezable(IndexManager::getInstance()->rioBlue)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Rio");
	}

	t = 0;
	t1 = 0;

	this->initExplosion(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY);

	this->initItem(this->sprite, IndexManager::getInstance()->en, NUM_FRAMES_BONUS, TIME_FRAME_DELAY);
	switch (color)
	{
	case Yellow:
		anim = new Animation(this->sprite, IndexManager::getInstance()->rioYellow, NUM_FRAMES_RIO, TIME_FRAME_DELAY);
		resetFrame = IndexManager::getInstance()->rioYellow[0];
		health = 4;
		break;

	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->rioBrown, NUM_FRAMES_RIO, TIME_FRAME_DELAY);
		resetFrame = IndexManager::getInstance()->rioBrown[0];
		health = 4;
		break;

	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->rioRed, NUM_FRAMES_RIO, TIME_FRAME_DELAY);
		resetFrame = IndexManager::getInstance()->rioRed[0];
		health = 6;
		break;

	default:
		break;
	}

	this->setOrigin(VECTOR2(0.5, 0.5));
	isHandle = true;
	isActivity = true;
	anim->start();
}

Rio::Rio()
{
}


Rio::~Rio()
{
	delete this->anim;
	delete this->sprite;
}
void Rio::reInit(VECTOR2 stP)
{
	this->startPosition = stP;
	this->P1 = this->startPosition;
	this->P3 = VECTOR2(P1.x + WIDTH_AREA_ACTIVE, startPosition.y + 17);
	this->P5 = VECTOR2(P3.x + WIDTH_AREA_ACTIVE, startPosition.y + 16);

	this->setPosition(stP);
	this->target = VECTOR2ZERO;
	this->sprite->setData(resetFrame);

	this->isActivity = true;
	isHandle = true;
	isCold = false;

	this->setPosition(startPosition);
	this->explosion->setPause(false);
	this->anim->setPause(false);

	IBonusable::reInit();
	IExplosible::reInit();
	this->setStatus(eStatus::START);
	health = 4;

}
void Rio::reInit()
{
	this->P1 = this->startPosition;
	this->P3 = VECTOR2(P1.x + WIDTH_AREA_ACTIVE, startPosition.y + 17);
	this->P5 = VECTOR2(P3.x + WIDTH_AREA_ACTIVE, startPosition.y + 16);

	this->target = VECTOR2ZERO;
	this->sprite->setData(resetFrame);

	this->isActivity = true;
	isHandle = true;
	isCold = false;
	this->setPosition(startPosition);
	this->anim->setPause(false);

	canDraw = true;
	IBonusable::reInit();
	IExplosible::reInit();
	this->setStatus(eStatus::START);
	t = 0;
	t1 = 0;
	start = false;
	health = 4;
}
void Rio::handleVelocity(float dt)
{
	if (isHandle && !this->isCold&&isActivity)
	{
		this->positionBefore = this->getPosition();
		this->positionAfter = this->getPosition();
		if (this->target != VECTOR2ZERO && this->isInStatus(eStatus::START))
		{
			if (P1.x < target.x && target.x < P3.x)
			{
				flag = 0;
				this->setStatus(eStatus::FALLING);
				start = true;
			}
			if (start == true && P3.x < target.x && target.x < P5.x)
			{
				flag = 1;
				this->setStatus(eStatus::FALLING);
			}

		}

		if (this->isInStatus(eStatus::FALLING))
		{
			if (flag == 0) // target is the left area
			{
				if (t < 1)
				{
					t += dt * RATE_BEZIER;
					this->P2 = VECTOR2((this->P1.x + this->P3.x) / 2, target.y - HEIGHT_AREA_ACTIVE);
					this->positionAfter = ((1 - t)*(1 - t)*P1 + 2 * (1 - t)*t*P2 + t*t*P3);
					if (P3.x < target.x && target.x < P5.x && this->positionAfter.x > P3.x - 1) {
						flag = 1;
						t = 0;
					}
					t1 = 0;
				}
				else
				{
					if (t1 < 1) {
						t1 += dt * RATE_BEZIER;
						this->P2 = VECTOR2((this->P3.x + this->P1.x) / 2, target.y - HEIGHT_AREA_ACTIVE);
						this->positionAfter = ((1 - t1)*(1 - t1)*P3 + 2 * (1 - t1)*t1*P2 + t1*t1*P1);
					}
					else {
						t = 0;
					}
				}
			}
			if (flag == 1)// target is the right area
			{
				if (t < 1)
				{
					t += dt * RATE_BEZIER;
					this->P4 = VECTOR2((this->P3.x + this->P5.x) / 2, target.y - HEIGHT_AREA_ACTIVE);
					this->positionAfter = ((1 - t)*(1 - t)*P3 + 2 * (1 - t)*t*P4 + t*t*P5);
					t1 = 0;
				}
				else
				{
					if (t1 < 1) {
						t1 += dt * RATE_BEZIER;
						this->P4 = VECTOR2((this->P3.x + this->P5.x) / 2, target.y - HEIGHT_AREA_ACTIVE);
						this->positionAfter = ((1 - t1)*(1 - t1)*P5 + 2 * (1 - t1)*t1*P4 + t1*t1*P3);
						if (P1.x < target.x && target.x < P3.x && positionAfter.x < P3.x + 1) {
							flag = 0;
							t1 = 0;
						}
					}
					else {
						t = 0;
					}
				}
			}

		}



		this->velocity = (positionAfter - positionBefore) / dt;
	}
	

}

void Rio::update(float dt)
{
	if (isHandle&&isActivity)
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
				//this->anim->setPause(true);
				this->setVelocity(VECTOR2(0, 0));
				return;
			}

		}
		if (beHit)
		{
			timerHit += dt;
			if (timerHit < TIME_DELAY_BE_HIT)
			{
				this->anim->setPause(true);
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
					this->isHandle = false;
					//this->isCold = false;
				}
			}
		}
		this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt));
		setBoundCollision();
		this->anim->update(dt);
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
			IBonusable::start();
		}
	}
	if (!Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->startBound)
		&& !Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->boundCollision))
	{
		reInit();
		isActivity = false;
	}
	else
	{
		isActivity = true;
	}
	
}

void Rio::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->getSprite()->getWidth() *0.5f;
	rect.right = position.x + this->getSprite()->getWidth() *0.5f;
	rect.top = position.y + this->getSprite()->getHeight() *0.5f;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f;

	this->boundCollision = rect;
}

bool Rio::getHandle()
{
	return this->isHandle;
}

void Rio::draw()
{
	if(canDraw)
		this->sprite->draw();
}

VECTOR2 Rio::getTarget()
{
	return this->target;
}
void Rio::setBeHit(bool hit)
{
	this->beHit = hit;
}
void Rio::decreaseHealth(float dame)
{
	this->health = this->health - dame;
}
void Rio::setStartPosition(VECTOR2 position)
{
	this->startPosition = position;
}

void Rio::setStartBound(MetroidRect rect)
{
	this->startBound = rect;
}

void Rio::setTarget(VECTOR2 target)
{
	this->target = target;
}


