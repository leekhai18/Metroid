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
	instance = this;
}


SamusStateManager::~SamusStateManager()
{
}

void SamusStateManager::init(Samus * samus, Input * input)
{
	stateStart = new SamusStateStart(samus, input);
	stateRunning = new SamusStateRunning(samus, input);
	stateStanding = new SamusStateStanding(samus, input);
	stateRolling = new SamusStateRolling(samus, input);
	stateJumping = new SamusStateJumping(samus, input);
	stateAcrobat = new SamusStateAcrobat(samus, input);
	stateFallRolling = new SamusStateFallRolling(samus, input);
	stateInjuring = new SamusStateInjuring(samus, input);

	currentState = stateStart;
}

void SamusStateManager::setOldStatus(eStatus status)
{
	this->oldStatus = status;
}

eStatus SamusStateManager::getOldStatus()
{
	return this->oldStatus;
}

void SamusStateManager::setOldState(BaseState * state)
{
	this->oldState = state;
}

BaseState * SamusStateManager::getOldState()
{
	return this->oldState;
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
	case eStatus::ACROBAT:
	{
		currentState = stateAcrobat;
		this->stateAcrobat->setJumpDistance(this->stateJumping->getJumpDistance());
		break;
	}
	case eStatus::FALLING_ROLLING:
		currentState = stateFallRolling;
		break;
	case eStatus::INJURING:

		currentState = stateInjuring;
		break;
	default:
		break;
	}


	currentState->init();
	currentState->onStart();
}

void SamusStateManager::release()
{
	delete stateStart;
	delete stateRolling;
	delete stateRunning;
	delete stateStanding;
	delete stateJumping;
	delete stateAcrobat;
	delete stateFallRolling;

	delete instance;
}
