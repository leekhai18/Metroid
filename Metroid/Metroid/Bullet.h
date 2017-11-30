#pragma once
#include "BaseObject.h"

#define DISTANCE_SHOOT 48.0f
#define TIME_TO_TARGET 0.2f
#define VELOCITY 240

class Bullet : public BaseObject
{
private:
	VECTOR2 startPosition;
	VECTOR2 target;

	// Use Bézier Curve
	float t;

public:
	Bullet(TextureManager* textureM, Graphics* graphics);
	Bullet();
	~Bullet();

	void update(float dt);
	void draw();

	void onCollision(BaseObject* object, CollisionReturn result);

	void setBoundCollision();
	void init(VECTOR2 stPosition, VECTOR2 target);

	// Use object pool
	void returnPool();
};

