#pragma once
#include "BaseState.h"

class SamusStateStanding : public BaseState
{
private:
	bool isUp;
	bool isShoot;

	float timerToRunning;

public:
	SamusStateStanding();
	SamusStateStanding(Samus* samus, Input* input);
	~SamusStateStanding();

	void init();
	void handleInput(float dt);
	void update(float dt);
	void setBoundCollision();
	void onStart();
	void onExit();

	void onCollision();

	void fire();
};

