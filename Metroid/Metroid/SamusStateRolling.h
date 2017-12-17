#pragma once
#include "BaseState.h"

class SamusStateRolling : public BaseState
{
private:
	float remain_time;
	float positionCollide;
	float time;

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
	void onCollision(float dt);
	void onStart();
	void onExit();
};

