#pragma once
#include "Scence.h"
#include "TextureManager.h"
class EndScence:public Scence
{
private:
	TextureManager* textureM;
	SpriteData data;
public:
	void init();

	void update();

	void draw();

	EndScence(Graphics* graphics, Input* input);
	void run();
	EndScence();
	~EndScence();
};

