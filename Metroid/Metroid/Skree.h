#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "SkreeEffectDeath.h"


#define TIME_FRAME_DELAY 0.15f
#define RATE_BEZIER 0.5f
#define X_OVER_HEAD 200
#define RATE_AREA_ACTIVE 0.15f
#define RATE_TIME_FRAME_UP 0.3f
#define EFFECT_DEATH_TIME 1.5f

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

