#pragma once
#include "Graphics.h"
#include "Input.h"
class Scence
{
protected:
	Graphics* graphics;
	Input* input;
public:
	Scence();
	Scence(Graphics* graphics,Input* input);
	virtual void init();
	virtual void update();
	virtual void draw();
	virtual void run();
	virtual void release();
	void handleLostGraphicsDevice();
	~Scence();
};

