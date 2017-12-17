#pragma once
#include "BaseState.h"
class SamusStateInjuring :public BaseState
{
private:
	float time_to_stand;
	float time_animation;

public:
	void init();
	void handleInput(float dt);
	void onCollision();
	void update(float dt);
	void onStart();
	void onExit();
	SamusStateInjuring();
	void setBoundCollision();
	SamusStateInjuring(Samus * samus, Input * input);
	~SamusStateInjuring();
	
};

