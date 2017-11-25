#pragma once
#include "BaseState.h"

class SamusStateRolling : public BaseState
{
public:
	SamusStateRolling();
	SamusStateRolling(Samus* samus, Input* input);
	~SamusStateRolling();


	void init();
	void handleInput(float dt);
	void update(float dt);

	void onStart();
	void onExit();
};

