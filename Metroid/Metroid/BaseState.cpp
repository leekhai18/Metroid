#include "BaseState.h"



BaseState::BaseState()
{
	this->samus = nullptr;
}

BaseState::BaseState(Samus * samus, Input* input)
{
	this->samus = samus;
	this->input = input;
}


BaseState::~BaseState()
{
}

void BaseState::setBoundCollision()
{
}

void BaseState::onCollision(BaseObject* object, CollisionReturn result)
{
}

