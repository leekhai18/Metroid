#pragma once
#include "BaseObject.h"
#include <list>

#define DISTANCE_SHOOT_ROCKET 100
#define VELOCITY_ROCKET 120
#define EXPLOSION_TIME_FRAME_DELAY 0.1f

class Rocket : public BaseObject
{
private:
	float distance;

	list<CollisionReturn> *listCollide;
	bool isCollided;
	float timer;

	float dame;

public:
	Rocket(TextureManager* textureM, Graphics* graphics);
	Rocket();
	~Rocket();

	void update(float dt);
	void draw();

	void onCollision();

	void setBoundCollision();
	void init(VECTOR2 stPosition);

	// Use object pool
	void returnPool();

	list<CollisionReturn> *getListCollide();
};

