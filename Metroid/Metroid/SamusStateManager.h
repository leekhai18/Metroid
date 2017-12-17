#pragma once
#include "SamusStateStart.h"
#include "SamusStateRunning.h"
#include "SamusStateStanding.h"
#include "SamusStateRolling.h"
#include "SamusStateJumping.h"
#include "SamusStateAcrobat.h"
#include "SamusStateFallRolling.h"
#include "SamusStateInjuring.h"
class SamusStateManager
{
private:
	static SamusStateManager* instance;

	SamusStateStart* stateStart;
	SamusStateRunning* stateRunning;
	SamusStateStanding* stateStanding;
	SamusStateRolling* stateRolling;
	SamusStateJumping* stateJumping;
	SamusStateAcrobat* stateAcrobat;
	SamusStateFallRolling* stateFallRolling;
	SamusStateInjuring* stateInjuring;
	BaseState* currentState;

	eStatus oldStatus;
	BaseState* oldState;
public:
	static SamusStateManager* getInstance();

	SamusStateManager();
	~SamusStateManager();
	void init(Samus* samus, Input* input);
	void setOldStatus(eStatus status);
	eStatus getOldStatus();
	void setOldState(BaseState* state);
	BaseState* getOldState();

	BaseState* getCurrentState();

	void changeStateTo(eStatus eStatus);


};

