#pragma once
#include "Sprite.h"
#include "IndexManager.h"

class BaseObject
{
protected:
	Sprite* sprite;
	eID id;
	eStatus status;

public:
	BaseObject(eID id);
	BaseObject();
	~BaseObject();

	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual void release() = 0;

	eID getId();
	eStatus getStatus();
	virtual void setStatus(eStatus status);
	virtual bool isInStatus(eStatus status);

	virtual GVector2 getPosition();
	virtual float getPositionX();
	virtual float getPositionY();

	virtual void setPosition(GVector2 position);
	virtual void setPosition(float x, float y);
	virtual void setPositionX(float x);
	virtual void setPositionY(float y);

	virtual void setScale(float scale);

	virtual float getRotate();
	virtual void setRotate(float degree);

	Sprite* getSprite();

	virtual float checkCollision(BaseObject* object, float dt);

	//virtual GVector2 getVelocity() = 0;

	//virtual void setPhysicsBodySide(eDirection side);
	//virtual eDirection getPhysicsBodySide();
};

