#pragma once
#include "BaseState.h"
class SamusStateInjuring :public BaseState
{
private:
	float time_to_stand;
	float time_animation;
	eStatus oldStatus;
public:
	void init();
	void handleInput(float dt);
	void onCollision(float dt);
	void update(float dt);
	void onStart();
	void onExit();
	void setOldStatus(eStatus status);
	eStatus getOldStatus();
	SamusStateInjuring();
	void setBoundCollision();
	SamusStateInjuring(Samus * samus, Input * input);
	~SamusStateInjuring();
	
};

