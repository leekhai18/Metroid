#include "GameManager.h"


// The primary class should inherit from Game class

//=============================================================================
// Constructor
//=============================================================================
GameManager::GameManager()
{
	input = new Input();        // initialize keyboard input immediately
								// additional initialization is handled in later call to input->initialize()
	paused = false;             // game is not paused
	graphics = NULL;
	initialized = false;
	fps = FRAME_RATE;
}

//=============================================================================
// Destructor
//=============================================================================
GameManager::~GameManager()
{
	deleteAll();                // free all reserved memory
	ShowCursor(true);           // show cursor
}

//=============================================================================
// Window message handler
//=============================================================================
LRESULT GameManager::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (initialized)     // do not process messages if not initialized
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
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);    // let Windows handle it
}

//=============================================================================
// Initializes the game
// throws GameError on error
//=============================================================================
void GameManager::initialize(HWND hw)
{
	hwnd = hw;                                  // save window handle

												// initialize graphics
	graphics = new Graphics();
	// throws GameError
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// initialize input, do not capture mouse
	input->initialize(hwnd, false);             // throws GameError

												// attempt to set up high resolution timer
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(GameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

	QueryPerformanceCounter(&timeStart);        // get starting time

	initialized = true;

	fps = FRAME_RATE;
}

//=============================================================================
// Render game items
//=============================================================================
void GameManager::renderGame()
{
	//start rendering
	if (SUCCEEDED(graphics->beginScene()))
	{
		graphics->spriteBegin();
		render();           // call render() in derived object
		graphics->spriteEnd();
							//stop rendering
		graphics->endScene();
	}
	handleLostGraphicsDevice();

	//display the back buffer on the screen
	graphics->showBackbuffer();
}

//=============================================================================
// Handle lost graphics device
//=============================================================================
void GameManager::handleLostGraphicsDevice()
{
	// test for and handle lost device
	hr = graphics->getDeviceState();
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
			releaseAll();
			hr = graphics->reset(); // attempt to reset graphics device
			if (FAILED(hr))          // if reset failed
				return;
			resetAll();
		}
		else
			return;                 // other device error
	}
}

//=============================================================================
// Toggle window or fullscreen mode
//=============================================================================
void GameManager::setDisplayMode(GraphicsNS::DISPLAY_MODE mode)
{
	releaseAll();                   // free all user created surfaces
	graphics->changeDisplayMode(mode);
	resetAll();                     // recreate surfaces
}

//=============================================================================
// Call repeatedly by the main message loop in WinMain
//=============================================================================
void GameManager::run(HWND hwnd)
{
	if (graphics == NULL)            // if graphics not initialized
		return;

	// calculate elapsed time of last frame, save in frameTime
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;

	// Power saving code, requires winmm.lib
	// if not enough time has elapsed for desired frame rate
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);         // Request 1mS resolution for windows timer
		Sleep(sleepTime);           // release cpu for sleepTime
		timeEndPeriod(1);           // End 1mS timer resolution
		return;
	}
	if (frameTime > MAX_FRAME_TIME) // if frame rate is very slow
		frameTime = MAX_FRAME_TIME; // limit maximum frameTime
	if (frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime);  // average fps
	timeStart = timeEnd;

	// update(), ai(), and collisions() are pure virtual functions.
	// These functions must be provided in the class that inherits from Game.
	if (!paused)                    // if not paused
	{
		update(frameTime);                   // update all game items
		ai();                       // artificial intelligence
		collisions();               // handle collisions
	}
	renderGame();                   // draw all game items

									// if Alt+Enter toggle fullscreen/window
	if (input->isKeyDown(ALT_KEY) && input->wasKeyPressed(ENTER_KEY))
		setDisplayMode(GraphicsNS::TOGGLE); // toggle fullscreen/window

											// if Esc key, set window mode
	if (input->isKeyDown(ESC_KEY))
		setDisplayMode(GraphicsNS::WINDOW); // set window mode

											// Clear input
											// Call this after all key checks are done
	input->clear(InputNS::KEYS_PRESSED);
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void GameManager::releaseAll()
{}

//=============================================================================
// Recreate all surfaces and reset all entities.
//=============================================================================
void GameManager::resetAll()
{}

//=============================================================================
// Delete all reserved memory
//=============================================================================
void GameManager::deleteAll()
{
	releaseAll();               // call onLostDevice() for every graphics item
	SAFE_DELETE(graphics);
	input = nullptr;
	delete input;
	initialized = false;
}
