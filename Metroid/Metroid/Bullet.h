#pragma once
#include "BaseObject.h"
#include <list>

#define DISTANCE_SHOOT 48.0f
#define VELOCITY 200

class Bullet : public BaseObject
{
private:
	float distance;

	list<CollisionReturn> *listCollide;

public:
	Bullet(TextureManager* textureM, Graphics* graphics);
	Bullet();
	~Bullet();

	void update(float dt);
	void draw();

	void onCollision();

	void setBoundCollision();
	void init(VECTOR2 stPosition);

	// Use object pool
	void returnPool();

	list<CollisionReturn> *getListCollide();
};

