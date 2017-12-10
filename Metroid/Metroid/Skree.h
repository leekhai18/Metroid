#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "SkreeEffectDeath.h"
#include <list>


class Skree : public BaseObject
{
private:
	Animation *animationRotate;
	SkreeEffectDeath *effectDeath;

	VECTOR2 target;

	float timerDeath;
	int health;

	// support to reinit
	VECTOR2 initPosition;
	bool isActivity;

public:
	Skree(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Skree();
	~Skree();

	void update(float dt);
	void draw();
	void release();

	VECTOR2 getTarget();
	void setTarget(VECTOR2 target);

	void setBoundCollision();

	// set init
	void setInitPosition(VECTOR2 pos);

	// reinit
	void reInit();

	void setActivity(bool flag);
	bool isActivitied();
};

