#include "IBonusable.h"



void IBonusable::initItem(Sprite * sprite, const int * frames, int totalFrames, float frameDelay)
{
	this->item = new Animation(sprite, frames, totalFrames, frameDelay, true);
	canDraw = true;
}

void IBonusable::reInit()
{
	this->item->setPause(false);
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
