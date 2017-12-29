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

class ScenceManager
{
private:
	map<ScenceType, Scence*>* scenceContainer;

	Scence* currentScence;

	HWND handle;

	Input* input;

	bool pause;

	Graphics* graphics;

	static ScenceManager* instance;
public:
	static ScenceManager* getInstance();

	Scence* getScence(ScenceType type);

	void goToScence(ScenceType type);


	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void insertScence(const char* name,Scence* scence);

	void run();


	void init(HWND hwnd);

	void deleteAll();

	ScenceManager();
	~ScenceManager();
};

