#pragma once
#include "BaseState.h"

class SamusStateJumping : public BaseState
{
private:
	bool isUp;
	float jumpDistance;
	float positionCollide;
public:
	SamusStateJumping();
	SamusStateJumping(Samus* samus, Input* input);
	~SamusStateJumping();

	void init();
	void handleInput(float dt);
	void update(float dt);

	void setBoundCollision();
	void onCollision();

	void onStart();
	void onExit();

	void fire();
};

