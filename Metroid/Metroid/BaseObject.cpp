#include "BaseObject.h"



BaseObject::BaseObject(eID id)
{
	this->id = id;
	this->sprite = nullptr;
	this->status = eStatus::START;
	this->direction = eDirection::right;
}

BaseObject::BaseObject()
{
	this->sprite = nullptr;
	this->status = eStatus::START;
	this->direction = eDirection::right;
}


BaseObject::~BaseObject()
{
}

void BaseObject::update(float dt)
{
}

void BaseObject::draw()
{
}

eID BaseObject::getId()
{
	return this->id;
}

eStatus BaseObject::getStatus()
{
	return this->status;
}

void BaseObject::setStatus(eStatus status)
{
	if (this->status != status)
		this->status = status;
}


bool BaseObject::isInStatus(eStatus status)
{
	if (this->getStatus() == status)
		return true;

	return false;
}

eDirection BaseObject::getDirection()
{
	return this->direction;
}

void BaseObject::setDirection(eDirection direct)
{
	if (this->direction != direct)
		this->direction = direct;
}

bool BaseObject::isInDirection(eDirection direct)
{
	if (this->direction == direct)
		return true;

	return false;
}

VECTOR2 BaseObject::getPosition()
{
	return this->sprite->getPosition();
}

void BaseObject::setPosition(VECTOR2 position)
{
	this->sprite->setPosition(position);
}

void BaseObject::setPosition(float x, float y)
{
	this->sprite->setPosition(VECTOR2(x, y));
}


void BaseObject::setPositionX(float x)
{
	this->sprite->setPositionX(x);
}

void BaseObject::setPositionY(float y)
{
	this->sprite->setPositionY(y);
}

VECTOR2 BaseObject::getScale()
{
	return this->sprite->getScale();
}

void BaseObject::setScale(VECTOR2 scale)
{
	this->sprite->setScale(scale);
}

void BaseObject::setScaleX(float scaleX)
{
	this->sprite->setScaleX(scaleX);
}

void BaseObject::setScaleY(float scaleY)
{
	this->sprite->setScaleY(scaleY);
}

VECTOR2 BaseObject::getOrigin()
{
	return this->sprite->getOrigin();
}

void BaseObject::setOrigin(VECTOR2 origin)
{
	this->sprite->setOrigin(origin);
}

float BaseObject::getRotate()
{
	return this->sprite->getRotate();
}

void BaseObject::setRotate(float rotate)
{
	this->sprite->setRotate(rotate);
}


Sprite * BaseObject::getSprite()
{
	return this->sprite;
}

void BaseObject::onCollision(BaseObject * object, float dt)
{

}

void BaseObject::setBoundCollision(MetroidRect rect)
{
	this->boundCollision = rect;
}

MetroidRect BaseObject::getBoundCollision()
{
	return boundCollision;
}

VECTOR2 BaseObject::getVelocity()
{
	return velocity;
}

void BaseObject::setVelocityX(float x)
{
	velocity.x = x;
}

void BaseObject::setVelocityY(float y)
{
	velocity.y = y;
}

void BaseObject::setVelocity(VECTOR2 velocity)
{
	this->velocity = velocity;
}