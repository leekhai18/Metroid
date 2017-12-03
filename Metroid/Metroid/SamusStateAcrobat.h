#pragma once
#include "BaseState.h"
class SamusStateAcrobat:public BaseState
{
private:
	float jumpDistance;
	float positionCollide;
	float time;
	float velocity_frame;
	//position that from jump state to acrobat
	//when we fall down to this position we will change to jump state
	float position_to_jump;
public:
	SamusStateAcrobat();
	SamusStateAcrobat(Samus * samus, Input * input);
	~SamusStateAcrobat();
	//void setPositonToJump(float position) { this->position_to_jump = position; }
	void setVelocityFrame(float velocity) { this->velocity_frame = velocity; }
	void setJumpDistance(float distance) { this->jumpDistance = distance; }
	void setBoundCollision();
	void init();
	void handleInput(float dt);
	void onCollision();
	void update(float dt);
	void onStart();
	void onExit();
};

