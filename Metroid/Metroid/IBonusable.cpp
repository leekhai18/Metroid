#include "IBonusable.h"
#include <stdlib.h>     
#include <time.h>      


void IBonusable::initItem(Sprite * sprite, const int * frames, int totalFrames, float frameDelay)
{
	this->item = new Animation(sprite, frames, totalFrames, frameDelay, true);
	canDraw = true;
}

void IBonusable::initRocket(Sprite * sprite, const int * frames, int totalFrames, float frameDelay)
{
	this->rocket = new Animation(sprite, frames, totalFrames, frameDelay, true);
}

void IBonusable::reInit()
{
	this->item->setPause(false);
	this->rocket->setPause(false);
	canDraw = true;
}
void IBonusable::setCanDraw(bool draw)
{
	this->canDraw = draw;
}
void IBonusable::start()
{
	this->item->start();
}

void IBonusable::start(bool isRocket)
{
	if(isRocket)
	{
		srand(time(NULL));
		int ran = rand() % 3 + 1;
		if (ran == 1)
		{
			this->rocket->start();
		}
		else
		{
			this->item->start();
		}
	}
	else
	{
		this->item->start();
	}
}

void IBonusable::update(float dt)
{
	this->item->update(dt);
}

IBonusable::IBonusable()
{
}


IBonusable::~IBonusable()
{
	delete item;
}
