#include "MotherBrainBullet.h"
#include "Collision.h"
#include "SamusStateManager.h"
#define RATE_BEZIER 1.5f
MotherBrainBullet::MotherBrainBullet(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::BULLET)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bullet");
	}

	// Set Data for sprite
	this->sprite->setData(IndexManager::getInstance()->samusYellowBulletNormal);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->setPosition(VECTOR2ZERO);
	this->setStatus(eStatus::ENDING);

	time = 0;

	isCreate = false;
	stop = false;
}

void MotherBrainBullet::setP1(VECTOR2 p1)
{
	this->P1 = p1;
	beforePosition = P1;
	afterPosition = P1;
	this->setPosition(P1);
}

void MotherBrainBullet::setP2(VECTOR2 p2)
{
	this->P2 = p2;
}

void MotherBrainBullet::setP3(VECTOR2 p3)
{
	this->P3 = p3;
}

bool MotherBrainBullet::getCreate()
{
	return this->isCreate;
}

bool MotherBrainBullet::getStop()
{
	return this->stop;
}

void MotherBrainBullet::setStop(bool stop)
{
	this->stop = stop;
}

void MotherBrainBullet::setCreate(bool isCreate)
{
	this->isCreate = isCreate;
}

void MotherBrainBullet::handleVelocity(float dt)
{
	if(!stop)
	{
		if (!isCreate)
		{
			beforePosition = afterPosition;

			time += dt * RATE_BEZIER;
			if (time >= maxTime)
			{
				time = maxTime;
				isCreate = true;
			}
			this->afterPosition = ((1 - time)*(1 - time)*P1 + 2 * (1 - time)*time*P2 + time*time*P3);

			this->velocity = (this->afterPosition - this->beforePosition) / dt;
		}
		else
		{
			beforePosition = afterPosition;

			this->afterPosition = ((1 - maxTime)*(1 - maxTime)*P1 + 2 * (1 - maxTime)*maxTime*P2 + maxTime*maxTime*P3);
			this->velocity = (this->afterPosition - this->beforePosition) / dt;

		}
	}
	
}

void MotherBrainBullet::update(float dt)
{
	this->setPosition(VECTOR2(this->getPosition().x + velocity.x*dt,this->getPosition().y +velocity.y*dt));
	setBoundCollision();
	if(this->isCreate)
	{
		this->velocity = VECTOR2(0, 0);
	}
}

void MotherBrainBullet::draw()
{
	this->sprite->draw();
}

void MotherBrainBullet::onCollision(Samus * samus, float dt)
{
	if (Collision::getInstance()->checkCollision(this, samus, dt)&& samus->getStatus()!=eStatus::INJURING)
	{
		SamusStateManager::getInstance()->setOldStatus(samus->getStatus());
		samus->setStatus(eStatus::INJURING);
		SamusStateManager::getInstance()->setOldState(SamusStateManager::getInstance()->getCurrentState());
	}
}


void MotherBrainBullet::setBoundCollision()
{
	boundCollision.left = getPosition().x - 1;
	boundCollision.right = getPosition().x + 1;
	boundCollision.top = getPosition().y + 1;
	boundCollision.bottom = getPosition().y - 1;
}

float MotherBrainBullet::getTime()
{
	return this->time;
}

void MotherBrainBullet::setTime(float time)
{
	this->time = time;
}

float MotherBrainBullet::getMaxTime()
{
	return this->maxTime;
}

void MotherBrainBullet::setMaxTime(float maxTime)
{
	this->maxTime = maxTime;
}

bool MotherBrainBullet::getVisible()
{
	return false;
}

void MotherBrainBullet::setVisible(bool visible)
{
}

MotherBrainBullet::MotherBrainBullet()
{
	
}


MotherBrainBullet::~MotherBrainBullet()
{
}
