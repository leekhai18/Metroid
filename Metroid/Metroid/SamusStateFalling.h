#pragma once
#include "BaseState.h"

class SamusStateFalling : public BaseState
{
public:
	SamusStateFalling();
	SamusStateFalling(Samus* samus, Input* input);
	~SamusStateFalling();

	void init();
	void handleInput(float dt);
	void update(float dt);

	void onStart();
	void onExit();
};

