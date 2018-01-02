#pragma once
#include "Sprite.h"

class Animation
{
private:
	const int* listFrames;
	int totalFrames;
	int currentFrame;
	int startFrame;
	bool canAnimate;
	float timerAnim;
	float frameDelay;
	bool isloop;
	bool isPause;
	bool isFinish;
	Sprite *sprite;

public:
	Animation(Sprite *sprite, const int* list, int totalFrames, float timeFrameDelay, bool loop);

	Animation(Sprite *sprite, const int* list, int totalFrames, float timeFrameDelay);
	~Animation();

	void reverse();
	void previousFrame();

	void nextFrame();

	void nextFrameReverse();
	void setValueOfCurrentFrame(int index);

	void setValueOfCurrentFrameReverse(int index);
	int getCurrentFrame();
	//use this update for animation not reverse
	void update(float dt);
	//use this for animation can be reversed
	void updateWithReverse(float dt);

	
	void start();
	void stop();

	
	void setPause(bool pause);
	bool isPausing();

	void reInit();
	bool isFinished();

	void setTimeFrameDelay(float time);
	float getTimeFrameDelay();

	void setListFrames(const int* list);
};

