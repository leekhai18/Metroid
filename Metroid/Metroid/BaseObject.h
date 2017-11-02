#pragma once
#include "Sprite.h"
#include "IndexManager.h"

class BaseObject
{
protected:
	Sprite* sprite;
	eID id;
	eStatus status;
	eDirection direction;

public:
	BaseObject(eID id);
	BaseObject();
	~BaseObject();

	virtual void handleInput(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual void release() = 0;

	eID getId();
	eStatus getStatus();
	virtual void setStatus(eStatus status);
	virtual bool isInStatus(eStatus status);

	eDirection getDirection();
	virtual void setDirection(eDirection direct);
	virtual bool isInDirection(eDirection direct);

	virtual VECTOR2 getPosition();
	virtual void setPosition(VECTOR2 position);
	virtual void setPosition(float x, float y);
	virtual void setPositionX(float x);
	virtual void setPositionY(float y);

	virtual VECTOR2 getScale();
	virtual void setScale(VECTOR2 scale);
	virtual void setScaleX(float scaleX);
	virtual void setScaleY(float scaleY);

	virtual VECTOR2 getOrigin();
	virtual void setOrigin(VECTOR2 origin);

	virtual float getRotate();
	virtual void setRotate(float rotate);

	Sprite* getSprite();

	virtual float checkCollision(BaseObject* object, float dt);

	//virtual GVector2 getVelocity() = 0;

	//virtual void setPhysicsBodySide(eDirection side);
	//virtual eDirection getPhysicsBodySide();
};

