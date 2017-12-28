#pragma once
#include "BaseObject.h"
#include <list>

#define DISTANCE_SHOOT 52
#define VELOCITY_BULLET 180
#define EXPLOSION_TIME_FRAME_DELAY 0.1f

class Bullet : public BaseObject
{
private:
	float distance;
	float distanceShoot;

	list<CollisionReturn> *listCollide;
	bool isCollided;
	float timer;

	float dame;

	int indexSprite;
	int indexEffect;

public:
	Bullet(TextureManager* textureM, Graphics* graphics);
	Bullet();
	~Bullet();

	void handleVelocity(float dt);
	void update(float dt);
	void draw();

	void onCollision();

	void setBoundCollision();
	void init(VECTOR2 stPosition);

	// Use object pool
	void returnPool();

	list<CollisionReturn> *getListCollide();

	float getDistanceShoot();
	void setDistanceShoot(float distan);

	void setIceBullet();
};

