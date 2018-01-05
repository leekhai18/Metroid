#include "Buble.h"
#include "Collision.h"
#include "Camera.h"
#define TIME_DELAY_BE_HIT 0.2f
#define BUBLE_VELOCITY_X 50
#define BUBLE_VELOCITY_Y 50
Buble::Buble()
{
}


Buble::~Buble()
{
}

Buble::Buble(TextureManager * textureM, Graphics * graphics, Samus * samus, EnemyColors color)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Waver");
	}
	this->samus = samus;
	initExplosion(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY);

	
	health = 4;

	this->sprite->setOrigin(VECTOR2(0.5, 0.5));

	isActivity = true;

	isHandle = true;

	timerHit = 0;
}



void Buble::setStartPosition(VECTOR2 position)
{
	startPosition = position;
}

void Buble::reInit()
{
	P1 = this->getPosition() - this->samus->getPosition();

	
	D3DXVec2Normalize(&normalize, &P1);
}

void Buble::setBeHit(bool hit)
{
	this->beHit = hit;
}

void Buble::decreaseHealth(float dame)
{
	this->health -= dame;
}

void Buble::handleVelocity(float dt)
{
	if(isActivity)
	{
		this->velocity = VECTOR2(normalize.x*BUBLE_VELOCITY_X, normalize.y*BUBLE_VELOCITY_Y);
	}
}

void Buble::setStartBound(MetroidRect rect)
{
	this->startBound = rect;
}

void Buble::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);

	rect.left = position.x - this->getSprite()->getWidth() *0.5f ;

	rect.right = position.x + this->getSprite()->getWidth()  *0.5f ;

	rect.top = position.y + this->getSprite()->getHeight()  *0.5f ;

	rect.bottom = position.y - this->getSprite()->getHeight()  *0.5f ;

	this->boundCollision = rect;
}

void Buble::update(float dt)
{
	if (isActivity)
	{
		if (beHit)
		{
			timerHit += dt;
			if (timerHit < TIME_DELAY_BE_HIT)
			{
				this->setVelocity(VECTOR2(0, 0));
			}
			else
			{
				timerHit = 0;
				beHit = false;
				if (this->health <= 0)
				{

					IExplosible::start();
					this->setVelocity(VECTOR2(0, 0));
					this->isHandle = false;
				}
			}
		}
		this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt));
		setBoundCollision();
	}

	if (!isExplose)
	{
		IExplosible::update(dt);
	}
	else
	{

	}
		
	if (!Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->startBound)
		&& !Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->boundCollision))
	{
		reInit();
	}
}

void Buble::onCollision(float dt)
{
	if(isActivity)
	{
	
	}
}

void Buble::draw()
{
	if (isActivity)
	{

	}
}

bool Buble::getHandle()
{
	return false;
}
