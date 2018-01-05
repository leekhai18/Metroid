#pragma once
#include "Animation.h"
#include "Sprite.h"
class IBonusable
{
protected:
	Animation *item;
	Animation *rocket;
	bool canDraw;
	bool isRocket;
public:
	void initItem(Sprite* sprite, const int* frames, int totalFrames, float frameDelay);
	void initRocket(Sprite* sprite, const int* frames, int totalFrames, float frameDelay);
	void reInit();
	void setCanDraw(bool draw, int *health, int *rocket);
	void start();
	void start(bool isRocket);
	void update(float dt);
	IBonusable();
	~IBonusable();
};

