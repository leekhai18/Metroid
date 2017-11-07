#include "ElementPool.h"


template<class EP>
void ElementPool<EP>::setUse(bool flag)
{
	if (this->inUse != flag)
		this->inUse = flag;
}

template<class EP>
bool ElementPool<EP>::isUsing()
{
	return this->inUse;
}

template<class EP>
void ElementPool<EP>::setValue(EP ep)
{
	this->ep = ep;
}

template<class EP>
EP ElementPool<EP>::getValue()
{
	return this->ep;
}
