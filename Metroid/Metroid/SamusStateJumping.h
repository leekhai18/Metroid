#pragma once
#include "BaseState.h"

class SamusStateJumping : public BaseState
{
private:
	bool isUp;

public:
	SamusStateJumping();
	SamusStateJumping(Samus* samus, Input* input);
	~SamusStateJumping();

	void init();
	void handleInput(float dt);
	void update(float dt);

	void onStart();
	void onExit();

	void fire();
};

