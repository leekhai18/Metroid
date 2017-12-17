#pragma once
#include "Constants.h"
#include "BaseObject.h"

class Collision
{
private:
	static Collision* instance;

public:
	static Collision * getInstance();


	//TO DO: true if one rectangle is colliding with another
	bool isCollide(MetroidRect myRect, MetroidRect otherRect);

	MetroidRect getSweptBroadphaseRect(MetroidRect myRect, VECTOR2 myVelocity, float frametime);

	//TO DO: true if one object will collide with another
	bool checkCollision(BaseObject* myObj, BaseObject* otherObj, float dt);

	//TO DO: calculate collision rect
	MetroidRect   CalculateCollisionRect(MetroidRect   rect1, MetroidRect   rect2);

	//TO DO: return collide area
	MetroidRect   getSweptBroadphaseRect(BaseObject* entity, float frametime);

	void release();

	Collision();
	~Collision();
};

