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
	stateRunning = nullptr;
	stateStart = nullptr;
	stateStanding = nullptr;
	stateRolling = nullptr;
	stateJumping = nullptr;
	currentState = nullptr;

	delete stateStart;
	delete stateRolling;
	delete stateRunning;
	delete stateStanding;
	delete stateJumping;
	delete currentState;
}

void SamusStateManager::init(Samus * samus, Input * input)
{
	stateStart = new SamusStateStart(samus, input);
	stateRunning = new SamusStateRunning(samus, input);
	stateStanding = new SamusStateStanding(samus, input);
	stateRolling = new SamusStateRolling(samus, input);
	stateJumping = new SamusStateJumping(samus, input);

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
		break;
	}
	case eStatus::RUNNING:
	{
		currentState = stateRunning;
		break;
	}
	case eStatus::STANDING:
	{
		currentState = stateStanding;
		break;
	}
	case eStatus::ROLLING:
	{
		currentState = stateRolling;
		break;
	}
	case eStatus::JUMPING:
	{
		currentState = stateJumping;
		break;
	}
	default:
		break;
	}


	currentState->init();
	currentState->onStart();
}
