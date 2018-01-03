#pragma once
#include "BaseState.h"

class SamusStateBoom:public BaseState
{
private:



	float timerToNormal;
public:
	SamusStateBoom();
	void setBoundCollision();
	SamusStateBoom(Samus * samus, Input * input);
	~SamusStateBoom();
	void init();
	void handleInput(float dt);
	void onCollision(float dt);
	void update(float dt);
	void onStart();
	void onExit();

};

