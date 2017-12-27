#include "IExplosible.h"



IExplosible::IExplosible()
{

}

void IExplosible::initExplosion(Sprite * sprite, const int * frames, int totalFrames, float frameDelay)
{
	this->explosion = new Animation(sprite, frames, totalFrames, frameDelay, false);
	

}



void IExplosible::reInit()
{
	this->isExplose = false;
	this->explosion->reInit();
	this->explosion->setPause(false);
}



void IExplosible::start()
{
	this->explosion->start();
}
void IExplosible::update(float dt)
{
	/*if(this->isExplose)
	{
		this->item->update(dt);
	}
	else
	{
		
	}*/
	this->explosion->update(dt);
	if (this->explosion->isFinished())
	{
		this->explosion->setPause(true);
		this->isExplose = true;
	}
	//this->canDraw = false;
}
bool IExplosible::getExplose()
{
	return this->isExplose;
}

IExplosible::~IExplosible()
{
	delete this->explosion;
	//delete this->item;
}




