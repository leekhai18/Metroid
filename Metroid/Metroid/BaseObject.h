#pragma once
#include "Sprite.h"
#include "IndexManager.h"

#define TIME_FRAME_ITEM 0.1f


class BaseObject
{
protected:
	Sprite* sprite;
	eID id;
	eStatus status;
	eDirection direction;
	MetroidRect activeBound;
	MetroidRect boundCollision;
	VECTOR2 velocity;
	VECTOR2 accelerate;
public:
	BaseObject(eID id);
	BaseObject();
	~BaseObject();

	virtual void update(float dt) ;
	virtual void draw() ;

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
	virtual void setAccelerate(VECTOR2 accelerate);
	virtual VECTOR2 getAccelerate();
	virtual VECTOR2 getOrigin();
	virtual void setOrigin(VECTOR2 origin);

	virtual float getRotate();
	virtual void setRotate(float rotate);
	
	Sprite* getSprite();

	//TO DO: update position, velocity... if this object collide with another object(call before update() )
	virtual void onCollision();

	virtual void setBoundCollision(MetroidRect rect);
	virtual MetroidRect getBoundCollision();

	virtual void setFlipX(bool flipX) { sprite->setFlipX(flipX); }
	virtual void setFlipY(bool flipY) { sprite->setFlipY(flipY); }
	virtual VECTOR2 getVelocity();
	virtual void setVelocityX(float x);
	virtual void setVelocityY(float y);
	virtual void setVelocity(VECTOR2 velocity);

	virtual MetroidRect getActiveBound();
	virtual void setActiveBound(MetroidRect rect);
};



struct CollisionReturn
{
	// top, left ,right ,bottom
	CollideDirection direction;
	//time two object collide
	float entryTime;
	//position of side that object collide. For example, when direction = TOP, it means positionCollision = otherBound.top
	float positionCollision;

	BaseObject* object;

	bool operator<(const CollisionReturn& collisionReturn) const
	{
		return this->entryTime < collisionReturn.entryTime;
	}
};