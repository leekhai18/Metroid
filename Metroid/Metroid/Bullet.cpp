#include "Bullet.h"
#include "BulletPool.h"
#include "Collision.h"

#define TIME_TO_TARGET 0.3f

Bullet::Bullet(TextureManager * textureM, Graphics * graphics) : BaseObject()
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bullet");
	}

	// Set Data for sprite
	this->sprite->setData(IndexManager::getInstance()->samusYellowBulletNormal);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->startPosition = VECTOR2ZERO;
	this->setPosition(VECTOR2ZERO);

	t = 0;
	timeToTar = TIME_TO_TARGET;

	this->setStatus(eStatus::START);

}

Bullet::Bullet()
{

}


Bullet::~Bullet()
{
	delete this->sprite;
}

void Bullet::setTimeToTar(float time)
{
	timeToTar = time;
}

float Bullet::getTimeToTar()
{
	return timeToTar;
}

void Bullet::update(float dt)
{
	if (t < 1)
	{
		t += dt * (1.0f / timeToTar);
		this->setPosition((1 - t)*this->startPosition + t*this->target);
		setBoundCollision();
	}
	else
	{
		this->setStatus(eStatus::ENDING);

		BulletPool::getInstance()->returnPool(this);
	}


}

void Bullet::draw()
{
	this->sprite->draw();
}

void Bullet::onCollision()
{
	list<CollisionReturn> dataReturn = Collision::getInstance()->getDataReturn();
	float addX = 0, addY = 0;
	for (list<CollisionReturn>::iterator object = dataReturn.begin(); object != dataReturn.end(); ++object)
	{
		eID objectID = (*object).idObject;
		CollideDirection collideDirection = (*object).direction;
		float entryTime = (*object).entryTime;
		float positionCollision = (*object).positionCollision;
		switch (objectID)
		{
		case eID::WALL:
			BulletPool::getInstance()->returnPool(this);
			break;
		default:
			break;
		}
	}
}

void Bullet::setBoundCollision()
{
	MetroidRect rect;

	rect.left = getPosition().x - WIDTH_BULLET*0.5f + 1;
	rect.right = getPosition().x + WIDTH_BULLET*0.5f - 1;
	rect.top = getPosition().y - HEIGHT_BULLET*0.5f + 1;
	rect.bottom = getPosition().y + HEIGHT_BULLET*0.5f - 1;

	boundCollision = rect;
}

void Bullet::init(VECTOR2 stPosition, VECTOR2 target)
{
	this->startPosition = stPosition;
	this->target = target;

	t = 0;
	this->setStatus(eStatus::RUNNING);
}

void Bullet::returnPool()
{
	this->startPosition = VECTOR2ZERO;
	this->target = VECTOR2ZERO;
}
