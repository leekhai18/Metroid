#pragma once
#include "BaseState.h"

class SamusStateStanding : public BaseState
{
private:
	bool isUp;
	bool isShoot;
	bool canRolling;

	float timerToRunning;

public:
	SamusStateStanding();
	SamusStateStanding(Samus* samus, Input* input);
	~SamusStateStanding();

	void init();
	void handleInput(float dt);
	void onCollision(float dt);

	void update(float dt);
	void setBoundCollision();
	void onStart();
	void onExit();

	void fire();
	void fireRocket();

};

