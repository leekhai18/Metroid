#include "BaseObject.h"



BaseObject::BaseObject(eID id)
{
	this->id = id;
	this->sprite = nullptr;
	this->setStatus(eStatus::NORMAL);
}

BaseObject::BaseObject()
{
	this->sprite = nullptr;
}


BaseObject::~BaseObject()
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

GVector2 BaseObject::getPosition()
{
	return this->sprite->getPosition();
}

float BaseObject::getPositionX()
{
	return this->sprite->getX();
}

float BaseObject::getPositionY()
{
	return this->sprite->getY();
}

void BaseObject::setPosition(GVector2 position)
{
	this->sprite->setPosition(position);
}

void BaseObject::setPosition(float x, float y)
{
	this->sprite->setPosition(GVector2(x, y));
}

void BaseObject::setPositionX(float x)
{
	this->sprite->setX(x);
}

void BaseObject::setPositionY(float y)
{
	this->sprite->setY(y);
}

void BaseObject::setScale(float scale)
{
	this->sprite->setScale(scale);
}

float BaseObject::getRotate()
{
	return this->sprite->getRadians();
}

void BaseObject::setRotate(float degree)
{
	this->sprite->setRadians(degree);
}

Sprite * BaseObject::getSprite()
{
	return this->sprite;
}


float BaseObject::checkCollision(BaseObject * object, float dt)
{
	return 0.0f;
}
