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
	this->samus = nullptr;
	delete this->samus;

	this->input = nullptr;
	delete this->input;

	this->animation = nullptr;
	delete this->animation;
}
