#pragma once
#include "GameManager.h"

class Metroid : public GameManager
{
public:
	Metroid();
	~Metroid();

	void initialize(HWND hwnd);
	void update();      // must override pure virtual from Game
	void ai();          // "
	void collisions();  // "
	void render();      // "
	void releaseAll();
	void resetAll();
};

