#pragma once
#include "BaseState.h"

class SamusStateRolling : public BaseState
{
private:
	float remain_time;
	float positionCollide;
	float time;
	//this is V(velocity.y) that is calculate every frame
	float velocity_frame;
	bool reset_fall;
	bool move_to_fall;

	bool canStanding;

	//bool isFall;

	//bool flagUpDown;
	//float timer;
public:
	SamusStateRolling();
	SamusStateRolling(Samus* samus, Input* input);
	~SamusStateRolling();

	void init();
	void handleInput(float dt);
	void update(float dt);
	void setBoundCollision();
	void onCollision();
	void onStart();
	void onExit();
};

