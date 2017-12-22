#include "IExplosible.h"



IExplosible::IExplosible()
{

}

void IExplosible::initialize(Sprite * sprite, const int * frames, int totalFrames, float frameDelay)
{
	this->explosion = new Animation(sprite, frames, totalFrames, frameDelay, false);
	canDraw = true;
}



void IExplosible::start()
{
	this->explosion->start();
}
void IExplosible::update(float dt)
{
	this->explosion->update(dt);
	if (this->explosion->isFinished())
		this->canDraw = false;
}
IExplosible::~IExplosible()
{
	delete this->explosion;
}




