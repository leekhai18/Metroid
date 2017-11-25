#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "SkreeEffectDeath.h"


class Skree : public BaseObject
{
private:
	Animation *animationRotate;
	SkreeEffectDeath *effectDeath;

	VECTOR2 startPosition;
	VECTOR2 target;

	// Use Bézier curve
	float t;

	float timerDeath;
public:
	Skree(TextureManager* textureM, Graphics* graphics);
	Skree();
	~Skree();

	void update(float dt);
	void draw();
	void release();

	VECTOR2 getTarget();
	void setTarget(VECTOR2 target);
};

