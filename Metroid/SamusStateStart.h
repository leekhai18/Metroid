#pragma once
#include "BaseState.h"

class SamusStateStart : public BaseState
{
public:
	SamusStateStart();
	SamusStateStart(Samus* samus, Input* input);
	~SamusStateStart();

	void init();
	void setBoundCollision();

	void handleInput(float dt);
	void update(float dt);

	void onStart();
	void onExit();
};

