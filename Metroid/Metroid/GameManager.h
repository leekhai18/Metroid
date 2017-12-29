#pragma once
#include <windows.h>
#include "Graphics.h"
#include "Input.h"

class GameManager
{
protected:

	Graphics *graphics;         // pointer to Graphics
	Input   *input;             // pointer to Input
	HWND    hwnd;               // window handle
	HRESULT hr;                 // standard return type
	LARGE_INTEGER timeStart;    // Performance Counter start value
	LARGE_INTEGER timeEnd;      // Performance Counter end value
	LARGE_INTEGER timerFreq;    // Performance Counter frequency
	float   deltaTime;          // time required for last frame
	float   fps;                // frames per second
	DWORD   sleepTime;          // number of milli-seconds to sleep between frames
	bool    paused;             // true if GameManager is paused
	bool    initialized;

public:
	GameManager();
	virtual ~GameManager();

	// Member functions

	// Window message handler
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Initialize the GameManager
	// Pre: hwnd is handle to window
	virtual void initialize(HWND hwnd);

	virtual void initialize(Graphics* graphics ,Input* input);
	// Call run repeatedly by the main message loop in WinMain
	virtual void run(HWND);
	virtual void run();
	// Call when the graphics device was lost.
	// Release all reserved video memory so graphics device may be reset.
	virtual void releaseAll();

	// Recreate all surfaces and reset all entities.
	virtual void resetAll();

	// Delete all reserved memory.
	virtual void deleteAll();

	// Render GameManager items.
	virtual void renderGame();

	// Handle lost graphics device
	virtual void handleLostGraphicsDevice();

	// Return pointer to Graphics.
	Graphics* getGraphics() { return graphics; }

	// Return pointer to Input.
	Input* getInput() { return input; }

	// Exit the Game
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	// Pure virtual function declarations
	// These functions MUST be written in any class that inherits from GameManager

	// Update Game items.
	virtual void update(float dt) = 0;

	virtual void handleInput(float dt) = 0;

	// Check for collisions.
	virtual void collisions(float dt) = 0;

	// Render graphics.
	// Call graphics->spriteBegin();
	//   draw sprites
	// Call graphics->spriteEnd();
	//   draw non-sprites
	virtual void render() = 0;

};

