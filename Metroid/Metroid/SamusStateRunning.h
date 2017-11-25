#pragma once
#include "BaseState.h"
#include "Animation.h"
#include "Collision.h"
class SamusStateRunning : public BaseState
{
private:
	Animation	*runningUp,
				*runningNormal,
				*runningHittingRight;

	bool isUp;
	
public:
	SamusStateRunning();
	SamusStateRunning(Samus* samus, Input* input);
	~SamusStateRunning();


	void init();
	void handleInput(float dt);
	void update(float dt);

	void setBoundCollision();
	void onCollision();

	void onStart();
	void onExit();

	void fire();
};

