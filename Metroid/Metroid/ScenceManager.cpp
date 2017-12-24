#include "ScenceManager.h"
#include "OptionScence.h"

ScenceManager* ScenceManager::instance;

ScenceManager * ScenceManager::getInstance()
{
	if(instance ==nullptr)
	{
		instance = new ScenceManager();
	}
	return instance;
}

Scence * ScenceManager::getScence(ScenceType)
{
	return nullptr;
}
void ScenceManager::goToScence(const char * name)
{
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
	update();
	draw();

	input->clear(InputNS::KEYS_PRESSED);
}

void ScenceManager::update()
{
	
}
void ScenceManager::draw()
{
	if (SUCCEEDED(graphics->beginScene()))
	{
		currentScence->draw();

		graphics->endScene();
	}
	handleLostGraphicsDevice();

	graphics->showBackbuffer();
}
void ScenceManager::handleLostGraphicsDevice()
{
	// test for and handle lost device
	HRESULT hr = graphics->getDeviceState();
	if (FAILED(hr))                  // if graphics device is not in a valid state
	{
		// if the device is lost and not available for reset
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);             // yield cpu time (100 mili-seconds)
			return;
		}
		// the device was lost but is now available for reset
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			//del();
			hr = graphics->reset(); // attempt to reset graphics device
			if (FAILED(hr))          // if reset failed
				return;
			//resetAll();
		}
		else
			return;                 // other device error
	}
}




void ScenceManager::init(HWND hwnd)
{
	this->handle = hwnd;
	input = new  Input();
	
	graphics = new Graphics();

	graphics->initialize(hwnd, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, FULLSCREEN);

	input->initialize(hwnd, false);

	this->scences = new map<ScenceType, Scence*>();
	OptionScence* optionS = new OptionScence(graphics, input);
	optionS->init();


	currentScence = optionS;
	this->scences->insert(pair<ScenceType, Scence*>(OPTION,optionS));
}

void ScenceManager::deleteAll()
{
	for (auto i = scences->begin(); i != scences->end(); ++i)
	{
		delete (*i).second;
	}
	delete scences;
	graphics->releaseAll();
	delete input;
	delete instance;
}

ScenceManager::ScenceManager()
{
}


ScenceManager::~ScenceManager()
{
}
