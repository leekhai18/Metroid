#include "Animation.h"


Animation::Animation(Sprite *sprite, const int* list, int totalFrames, float timeAnim, bool loop)
{
	this->sprite = sprite;
	this->listFrames = list;
	this->totalFrames = totalFrames;
	this->currentFrame = -1;
	this->startFrame = 0;
	this->endFrame = totalFrames;
	this->isloop = loop;
	this->frameDelay = timeAnim;
	this->timerAnim = 0;
	this->canAnimate = false;
}

Animation::Animation(Sprite *sprite, const int * list, int totalFrames, float timeAnim)
{
	this->sprite = sprite;
	this->listFrames = list;
	this->totalFrames = totalFrames;
	this->currentFrame = -1;
	this->startFrame = 0;
	this->endFrame = totalFrames;
	this->frameDelay = timeAnim;
	this->timerAnim = 0;
	this->canAnimate = false;
}

Animation::Animation()
{
}


Animation::~Animation()
{
	delete this->sprite;
}

void Animation::nextFrame()
{
	this->setValueOfCurrentFrame(currentFrame + 1);
}

void Animation::setValueOfCurrentFrame(int index)
{
	if (index == currentFrame || totalFrames == 0)
		return;

	currentFrame = index;

	if (currentFrame > endFrame)
		currentFrame = startFrame;

	// setRect
	this->sprite->setSpriteDataRect(SpriteManager::getInstance()->getSpritesData()[this->listFrames[currentFrame]].rect);

	//if (!loop && currentFrame == endFrame)
	//{
	//	this->stop();
	//}
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
	}
}

void Animation::stop()
{
	if (canAnimate == true)
	{
		canAnimate = false;
	}
}
