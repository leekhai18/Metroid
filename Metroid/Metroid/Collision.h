#pragma once
#include "Constants.h"
#include "BaseObject.h"
#include "ObjectManager.h"
class Collision
{
private:
	static Collision* instance;
	CollideDirection collideDirection;
	list<CollisionReturn> dataReturn;
public:
	static Collision * getInstance();
	//TO DO: true if object is on ground
	bool checkOnGround(MetroidRect obj);
	//TO DO: true if one rectangle is colliding with another
	bool isCollide(MetroidRect myRect, MetroidRect otherRect);
	//TO DO: true if one object will collide with another
	bool checkCollision(BaseObject* myObj, BaseObject* otherObj, float dt);
	//TO DO: calculate collision rect
	MetroidRect   CalculateCollisionRect(MetroidRect   rect1, MetroidRect   rect2);
	//TO DO: return collide area
	MetroidRect   getSweptBroadphaseRect(BaseObject* entity, float frametime);

	list<CollisionReturn>& getDataReturn();
	void clearDataReturn();
	Collision();
	void release();
	~Collision();
};

