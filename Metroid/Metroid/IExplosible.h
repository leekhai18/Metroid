#pragma once
#include "Animation.h"
#include "Sprite.h"
class IExplosible
{
protected:
	Animation *explosion;

	bool isExplose;

public:
	IExplosible();
	void initExplosion(Sprite* sprite, const int* frames, int totalFrames, float frameDelay);

	void reInit();
	void start();
	void update(float dt);
	bool getExplose();
	~IExplosible();
};

