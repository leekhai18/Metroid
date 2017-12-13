#pragma once
#include "BaseState.h"

class SamusStateFallRolling:public BaseState
{
private:
	float remain_time;
	//this is V(velocity.y) that is calculate every frame
	float velocity_frame;
public:
	SamusStateFallRolling();
	SamusStateFallRolling(Samus* samus, Input* input);
	~SamusStateFallRolling();


	void init();
	void handleInput(float dt);
	void onCollision();
	void update(float dt);
	void setBoundCollision();

	void onStart();
	void onExit();
};

