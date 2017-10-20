#pragma once
#include <windows.h>
#include "Graphics.h"
#include "Input.h"

class GameManager
{
protected:
	// common GameManager properties
	Graphics *graphics;         // pointer to Graphics
	Input   *input;             // pointer to Input
	HWND    hwnd;               // window handle
	HRESULT hr;                 // standard return type
	LARGE_INTEGER timeStart;    // Performance Counter start value
	LARGE_INTEGER timeEnd;      // Performance Counter end value
	LARGE_INTEGER timerFreq;    // Performance Counter frequency
	float   frameTime;          // time required for last frame
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

	// Call run repeatedly by the main message loop in WinMain
	virtual void run(HWND);

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

	// Set display mode (fullscreen, window or toggle)
	void setDisplayMode(GraphicsNS::DISPLAY_MODE mode = GraphicsNS::TOGGLE);

	// Return pointer to Graphics.
	Graphics* getGraphics() { return graphics; }

	// Return pointer to Input.
	Input* getInput() { return input; }

	// Exit the Game
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	// Pure virtual function declarations
	// These functions MUST be written in any class that inherits from GameManager

	// Update Game items.
	virtual void update() = 0;

	// Perform AI calculations.
	virtual void ai() = 0;

	// Check for collisions.
	virtual void collisions() = 0;

	// Render graphics.
	// Call graphics->spriteBegin();
	//   draw sprites
	// Call graphics->spriteEnd();
	//   draw non-sprites
	virtual void render() = 0;

};

