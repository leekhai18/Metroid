#pragma once
#include "BaseState.h"
#include "Animation.h"

class SamusStateRunning : public BaseState
{
private:
	Animation	*runningUp,
				*runningNormal,
				*runningShoot;

public:
	SamusStateRunning();
	SamusStateRunning(Samus* samus, Input* input);
	~SamusStateRunning();


	void init();
	void handleInput(float dt);
	void update(float dt);

	void onStart();
	void onExit();
};

