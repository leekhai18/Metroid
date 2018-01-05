#pragma once
#include "BaseState.h"

class SamusStateJumping : public BaseState
{
private:
	bool isUp;
	float jumpDistance;
	float positionCollide;
	float time;
public:
	SamusStateJumping();
	SamusStateJumping(Samus* samus, Input* input);
	~SamusStateJumping();
	float getJumpDistance() { return jumpDistance; }
	void init();
	void handleInput(float dt);
	void update(float dt);

	void setBoundCollision();
	void onCollision(float dt);

	void onStart();
	void onExit();

	void fire();
	void fireRocket();
};

