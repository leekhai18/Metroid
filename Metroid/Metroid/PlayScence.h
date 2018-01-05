#pragma once
#include "Scence.h"
#include "Metroid.h"
class PlayScence:public Scence
{
private:
	Metroid* metroidGame;
public:
	void init();
	void update();
	void draw();
	void run();
	void release();
	PlayScence(Graphics* graphics, Input* input);
	PlayScence();
	~PlayScence();
};

