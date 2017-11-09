#pragma once
#include "BaseState.h"

class SamusStateStanding : public BaseState
{
private:
	bool isUp;

	float timerToRunning;

public:
	SamusStateStanding(); 
	SamusStateStanding(Samus* samus, Input* input);
	~SamusStateStanding();

	void init();
	void handleInput(float dt);
	void update(float dt);

	void onStart();
	void onExit();

	void fire();
};

