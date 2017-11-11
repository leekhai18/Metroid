#pragma once
#include "BaseObject.h"
#include "Animation.h"


#define TIME_FRAME_DELAY 0.15f

class Zommer: public BaseObject
{
private:
	Animation *animation;
	bool flag = true;
	int change = 1;

public:
	Zommer(TextureManager* textureM, Graphics* graphics);
	Zommer();
	~Zommer();

	void update(float dt);
	void draw();
	void release();
};

