#pragma once
#include <Windows.h>
#include <stdlib.h>             // for detecting memory leaks
#include <crtdbg.h>             // for detecting memory leaks
#include "gameError.h"
#include "constants.h"
#include "Metroid.h"
#include "resource.h"
#include "Scence.h"
#include <map>
#include "Graphics.h"
class ScenceManager
{
private:
	map<ScenceType, Scence*>* scences;

	Scence* currentScence;

	HWND handle;

	Input* input;

	bool pause;

	Graphics* graphics;

	static ScenceManager* instance;
public:
	static ScenceManager* getInstance();

	Scence* getScence(ScenceType);

	void goToScence(const char* name);

	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void insertScence(const char* name,Scence* scence);

	void run();

	void update();

	void handleLostGraphicsDevice();

	void draw();

	void init(HWND hwnd);

	void deleteAll();

	ScenceManager();
	~ScenceManager();
};

