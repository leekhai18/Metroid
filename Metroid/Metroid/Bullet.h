#pragma once
#include "BaseObject.h"
#include "Collision.h"

#define WIDTH_BULLET 4
#define HEIGHT_BULLET 4
#define DISTANCE_SHOOT 100.0f
class Bullet : public BaseObject
{
private:
	VECTOR2 startPosition;
	VECTOR2 target;

	float timeToTar;

	// Use Bézier Curve
	float t;

public:
	Bullet(TextureManager* textureM, Graphics* graphics);
	Bullet();
	~Bullet();

	void setTimeToTar(float time);
	float getTimeToTar();



	void update(float dt);
	void draw();

	void onCollision();

	void setBoundCollision();
	void init(VECTOR2 stPosition, VECTOR2 target);

	// Use object pool
	void returnPool();
};

