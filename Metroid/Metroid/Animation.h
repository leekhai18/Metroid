#pragma once
#include "Sprite.h"

class Animation
{
private:
	const int* listFrames;
	int totalFrames;
	int currentFrame;
	int startFrame;
	int endFrame;
	bool canAnimate;
	float timerAnim;
	float frameDelay;
	bool isloop;
	Sprite *sprite;
public:
	Animation(Sprite *sprite, const int* list, int totalFrames, float timeAnim, bool loop);
	Animation(Sprite *sprite, const int* list, int totalFrames, float timeAnim);
	Animation();
	~Animation();

	void nextFrame();
	void setValueOfCurrentFrame(int index);
	int getCurrentFrame();
	void update(float dt);
	void start();
	void stop();
};

