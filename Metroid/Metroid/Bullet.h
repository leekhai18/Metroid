#pragma once
#include "BaseObject.h"


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

	void init(VECTOR2 stPosition, VECTOR2 target);

	// Use object pool
	void returnPool();
};

