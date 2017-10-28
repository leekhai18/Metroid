#include "Animation.h"


Animation::Animation(Sprite *sprite, const int* list, int totalFrames, float timeAnim, bool loop)
{
	this->sprite = sprite;
	this->listFrames = list;
	this->totalFrames = totalFrames;
	this->currentFrame = -1;
	this->startFrame = 0;
	this->isloop = loop;
	this->isCompleted = false;
	this->frameDelay = timeAnim;
	this->timerAnim = 0;
	this->canAnimate = false;
	
	// set width, height
	this->sprite->setSpriteWidth(SpriteManager::getInstance()->getSpritesData()[this->listFrames[0]].width);
	this->sprite->setSpriteHeigth(SpriteManager::getInstance()->getSpritesData()[this->listFrames[0]].height);
}

Animation::Animation(Sprite *sprite, const int * list, int totalFrames, float timeAnim)
{
	this->sprite = sprite;
	this->listFrames = list;
	this->totalFrames = totalFrames;
	this->currentFrame = -1;
	this->startFrame = 0;
	this->frameDelay = timeAnim;
	this->timerAnim = 0;
	this->canAnimate = false;
	this->isCompleted = false;
	this->isloop = true;

	// set width, height
	this->sprite->setSpriteWidth(SpriteManager::getInstance()->getSpritesData()[this->listFrames[0]].width);
	this->sprite->setSpriteHeigth(SpriteManager::getInstance()->getSpritesData()[this->listFrames[0]].height);
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
	if (totalFrames == 0)
		return;

	currentFrame = index;

	if (currentFrame > totalFrames - 1)
		currentFrame = startFrame;

	// setRect
	this->sprite->setSpriteDataRect(SpriteManager::getInstance()->getSpritesData()[this->listFrames[currentFrame]].rect);

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
