#include "ScenceManager.h"
#include "OptionScence.h"
#include "PlayScence.h"
#include "Constants.h"
#include "EndScence.h"
ScenceManager* ScenceManager::instance;

ScenceManager * ScenceManager::getInstance()
{
	if(instance ==nullptr)
	{
		instance = new ScenceManager();
	}
	return instance;
}

Scence * ScenceManager::getScence(ScenceType type)
{
	return (*scenceContainer->find(type)).second;
}
void ScenceManager::goToScence(ScenceType type)
{
	currentScence = (*scenceContainer->find(type)).second;
}
LRESULT ScenceManager::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);					//tell Windows to kill this program
		return 0;
	case WM_KEYDOWN: case WM_SYSKEYDOWN:    // key down
		input->keyDown(wParam);
		return 0;
	case WM_KEYUP: case WM_SYSKEYUP:        // key up
		input->keyUp(wParam);
		return 0;
	case WM_CHAR:                           // character entered
		input->keyIn(wParam);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);    // let Windows handle it
}
void ScenceManager::insertScence(const char * name, Scence * scence)
{

}

void ScenceManager::run()
{
	this->currentScence->run();
	input->clear(InputNS::KEYS_PRESSED);
	if(input->isKeyDown(VK_RETURN))
	{
		this->goToScence(ScenceType::PLAY);
	}
}

void ScenceManager::init(HWND hwnd)
{
	this->handle = hwnd;
	input = new  Input();
	
	graphics = new Graphics();

	graphics->initialize(hwnd, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, FULLSCREEN);

	input->initialize(hwnd, false);

	this->scenceContainer = new map<ScenceType, Scence*>();
	OptionScence* optionS = new OptionScence(graphics, input);
	optionS->init();
	PlayScence* playS = new PlayScence(graphics, input);
	EndScence* endS = new EndScence(graphics, input);
	endS->init();

	currentScence = optionS;
	this->scenceContainer->insert(pair<ScenceType, Scence*>(ScenceType::OPTION,optionS));
	this->scenceContainer->insert(pair<ScenceType, Scence*>(ScenceType::PLAY, playS) );
	this->scenceContainer->insert(pair<ScenceType, Scence*>(ScenceType::END, endS));
}

void ScenceManager::deleteAll()
{
	for (auto i = scenceContainer->begin(); i != scenceContainer->end(); ++i)
	{
		delete (*i).second;
	}
	delete scenceContainer;
	graphics->releaseAll();
	ShowCursor(true);
	delete input;
	delete instance;
}

HWND ScenceManager::getHWnd()
{
	return this->handle;
}

ScenceManager::ScenceManager()
{
}


ScenceManager::~ScenceManager()
{
}
