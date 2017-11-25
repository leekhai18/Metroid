#pragma once
#include "Constants.h"
#include "BaseObject.h"
#include "ObjectManager.h"
class Collision
{
private:
	static Collision* instance;	
	CollideDirection collideDirection;
public:
	static Collision * getInstance();
	//TO DO: true if object is on ground
	bool checkOnGround(MetroidRect obj,float distance);
	//TO DO: true if one rectangle is colliding with another
	bool isCollide(MetroidRect myRect, MetroidRect otherRect);
	//TO DO: true if one object will collide with another
	bool checkCollision(BaseObject* myObj, BaseObject* otherObj ,float frametime, CollideDirection& direction, MetroidRect& collideRect);
	//TO DO: calculate collision rect
	MetroidRect   CalculateCollisionRect(MetroidRect   rect1, MetroidRect   rect2);
	//TO DO: return collide area
	MetroidRect   getSweptBroadphaseRect(BaseObject* entity, float frametime);
	Collision();
	~Collision();

	void release();
};

