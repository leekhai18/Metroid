#pragma once
#include "BaseState.h"

class SamusStateRolling : public BaseState
{
private:
	bool isFall;

	bool flagUpDown;
	float timer;

public:
	SamusStateRolling();
	SamusStateRolling(Samus* samus, Input* input);
	~SamusStateRolling();

	void init();
	void handleInput(float dt);
	void update(float dt);
	void setBoundCollision();
	void onCollision();
	void onStart();
	void onExit();
};

