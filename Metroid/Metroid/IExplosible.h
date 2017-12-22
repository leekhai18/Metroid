#pragma once
#include "Animation.h"
#include "Sprite.h"
class IExplosible
{
protected:
	Animation *explosion;
	bool canDraw;
public:
	IExplosible();
	void initialize(Sprite* sprite, const int* frames, int totalFrames, float frameDelay);
	void start();
	void update(float dt);
	~IExplosible();
};

