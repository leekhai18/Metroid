#pragma once
#include "BaseState.h"

class SamusStateJumping : public BaseState
{
private:
	bool isUp;
	float jumpDistance;
	float positionCollide;
	float time;
	//this is V(velocity.y) that is calculate every frame
	float velocity_frame;
public:
	SamusStateJumping();
	SamusStateJumping(Samus* samus, Input* input);
	~SamusStateJumping();
	float getVelocity_Frame() { return velocity_frame; }
	float getJumpDistance() { return jumpDistance; }
	void init();
	void handleInput(float dt);
	void update(float dt);

	void setBoundCollision();
	void onCollision();

	void onStart();
	void onExit();

	void fire();
};

