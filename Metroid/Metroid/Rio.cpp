#include "Rio.h"
#define RATE_BEZIER 0.45f
#define TIME_FRAME_DELAY 0.15f
#define WIDTH_AREA_ACTIVE 100
#define HEIGHT_AREA_ACTIVE 130
#define VELOCITY_X 60

Rio::Rio(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::RIO), IFreezable(IndexManager::getInstance()->rioBlue)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Rio");
	}

	t = 0;
	t1 = 0;


	switch (color)
	{
	case Yellow:
		anim = new Animation(this->sprite, IndexManager::getInstance()->rioYellow, NUM_FRAMES_RIO, TIME_FRAME_DELAY);
		health = 4;
		break;

	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->rioBrown, NUM_FRAMES_RIO, TIME_FRAME_DELAY);
		health = 4;
		break;

	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->rioRed, NUM_FRAMES_RIO, TIME_FRAME_DELAY);
		health = 6;
		break;

	default:
		break;
	}

	this->setOrigin(VECTOR2(0.5, 0.5));
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

void Rio::update(float dt)
{
	if (this->isCold)
	{
		this->sprite->setData(this->frameID[anim->getCurrentFrame()]);
		this->anim->setPause(true);
		return;
	}
	else
	{
		this->anim->setPause(false);
	}

	this->anim->update(dt);

	positionBeforeX = this->getPosition().x;
	positionBeforeY = this->getPosition().y;

	if (this->target != VECTOR2ZERO && this->isInStatus(eStatus::START))
	{
		if (P1.x < target.x && target.x < P3.x )
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
				this->setPosition((1 - t)*(1 - t)*P1 + 2 * (1 - t)*t*P2 + t*t*P3);
				if (P3.x < target.x && target.x < P5.x && this->getPosition().x > P3.x - 1) {
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
					this->setPosition((1 - t1)*(1 - t1)*P3 + 2 * (1 - t1)*t1*P2 + t1*t1*P1);
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
				this->setPosition((1 - t)*(1 - t)*P3 + 2 * (1 - t)*t*P4 + t*t*P5);
				t1 = 0;
			}
			else
			{
				if (t1 < 1) {
					t1 += dt * RATE_BEZIER;
					this->P4 = VECTOR2((this->P3.x + this->P5.x) / 2, target.y - HEIGHT_AREA_ACTIVE);
					this->setPosition((1 - t1)*(1 - t1)*P5 + 2 * (1 - t1)*t1*P4 + t1*t1*P3);
					if (P1.x < target.x && target.x < P3.x && this->getPosition().x < P3.x + 1) {
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

	positionAfterX = this->getPosition().x;
	positionAfterY = this->getPosition().y;
	this->velocity.x = (positionAfterX - positionBeforeX) / dt;
	this->velocity.y = (positionAfterY - positionBeforeY) / dt;
	
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

void Rio::draw()
{
	this->sprite->draw();
}

VECTOR2 Rio::getTarget()
{
	return this->target;
}

void Rio::setTarget(VECTOR2 target)
{
	this->target = target;
}

void Rio::initPositions(VECTOR2 stP)
{
	this->startPosition = stP;
	this->P1 = this->startPosition;
	this->P3 = VECTOR2(P1.x + WIDTH_AREA_ACTIVE, startPosition.y + 17);
	this->P5 = VECTOR2(P3.x + WIDTH_AREA_ACTIVE, startPosition.y + 16);

	this->setPosition(stP);
	this->target = VECTOR2ZERO;
}
