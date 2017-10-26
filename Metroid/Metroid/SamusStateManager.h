#pragma once
#include "SamusStateStart.h"

class SamusStateManager
{
private:
	static SamusStateManager* instance;

	SamusStateStart* stateStart;
	BaseState* currentState;

public:
	static SamusStateManager* getInstance();

	SamusStateManager();
	~SamusStateManager();
	void init(Samus* samus, Input* input);

	BaseState* getCurrentState();

	void changeStateTo(eStatus eStatus);

};

