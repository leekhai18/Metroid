#include "SamusStateManager.h"

SamusStateManager* SamusStateManager::instance = nullptr;

SamusStateManager * SamusStateManager::getInstance()
{
	if (instance == nullptr)
		instance = new SamusStateManager();

	return instance;
}

SamusStateManager::SamusStateManager()
{
	stateStart = new SamusStateStart();
	currentState = stateStart;
}


SamusStateManager::~SamusStateManager()
{
	delete stateStart;
	delete currentState;
}

void SamusStateManager::init(Samus * samus, Input * input)
{
	stateStart = new SamusStateStart(samus, input);
	currentState = stateStart;
}

BaseState * SamusStateManager::getCurrentState()
{
	return this->currentState;
}

void SamusStateManager::changeStateTo(eStatus eStatus)
{
	if (currentState != nullptr)
		currentState->onExit();

	switch (eStatus)
	{
	case eStatus::START:
	{
		currentState = stateStart;
	}
	default:
		break;
	}


	currentState->init();
	currentState->onStart();
}
