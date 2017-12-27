#include "IFreezable.h"



bool IFreezable::getCold()
{
	return this->isCold;
}

void IFreezable::setCold(bool cold)
{
	this->isCold = cold;
}

IFreezable::IFreezable()
{
}

IFreezable::IFreezable(const int * frameID)
{
	this->frameID = frameID;
	isCold = false;
}



IFreezable::~IFreezable()
{
}
