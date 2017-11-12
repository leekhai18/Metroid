#include "Animation.h"


Animation::Animation(Sprite *sprite, const int* list, int totalFrames, float timeFrameDelay, bool loop)
{
	this->sprite = sprite;
	this->listFrames = list;
	this->totalFrames = totalFrames;
	this->currentFrame = 0;
	this->startFrame = 0;
	this->isloop = loop;
	this->isCompleted = false;
	this->frameDelay = timeFrameDelay;
	this->timerAnim = 0;
	this->canAnimate = false;

	// Get data of sprite
	this->sprite->setData(this->listFrames[0]);
}

Animation::Animation(Sprite *sprite, const int * list, int totalFrames, float timeFrameDelay)
{
	this->sprite = sprite;
	this->listFrames = list;
	this->totalFrames = totalFrames;
	this->currentFrame = 0;
	this->startFrame = 0;
	this->frameDelay = timeFrameDelay;
	this->timerAnim = 0;
	this->canAnimate = false;
	this->isCompleted = false;
	this->isloop = true;

	// Get data of sprite
	this->sprite->setData(this->listFrames[0]);
}


Animation::~Animation()
{
}

void Animation::nextFrame()
{
	this->setValueOfCurrentFrame(currentFrame + 1);
}

void Animation::setValueOfCurrentFrame(int index)
{
	if (totalFrames == 0)
		return;

	currentFrame = index;

	if (currentFrame > totalFrames - 1)
		currentFrame = startFrame;

	// Get data of sprite
	this->sprite->setData(this->listFrames[currentFrame]);

	if (!isloop && currentFrame == totalFrames - 1)
	{
		this->stop();
	}
}

int Animation::getCurrentFrame()
{
	return currentFrame;
}

void Animation::update(float dt)
{
	if (!canAnimate)
		return;

	timerAnim += dt;
	if (timerAnim > frameDelay)
	{
		if (canAnimate)
			this->nextFrame();

		timerAnim -= frameDelay;
	}
}

void Animation::start()
{
	if (canAnimate == false)
	{
		canAnimate = true;
		isCompleted = false;
	}
}

void Animation::stop()
{
	if (canAnimate == true)
	{
		canAnimate = false;
		isCompleted = true;
	}
}

bool Animation::isFinished()
{
	return isCompleted;
}

void Animation::setTimeFrameDelay(float time)
{
	if (this->frameDelay != time)
		this->frameDelay = time;
}

float Animation::getTimeFrameDelay()
{
	return this->frameDelay;
}
