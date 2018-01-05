#pragma once
#include "Scence.h"
#include "TextureManager.h"
#include "Sprite.h"
class OptionScence :public Scence
{
private:
	TextureManager* textureM;
	SpriteData data;
public:
	void init();
	void update();
	void draw();
	void run();
	void release();
	OptionScence();
	OptionScence(Graphics* graphics, Input* input);
	~OptionScence();
};

