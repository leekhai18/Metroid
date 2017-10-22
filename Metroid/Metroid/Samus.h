#pragma once
#include "Sprite.h"
#include "Graphics.h"
#include "Input.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "GameError.h"
#include "Animation.h"
#include "BaseObject.h"



class Samus: public BaseObject
{
private:
	Input* input;
	Animation	*currentAnimation,
				*runningAnimation,
				*rollingAnimation;



public:
	Samus(TextureManager* textureM, Graphics* graphics, Input* input);
	Samus();
	~Samus();

	void setRect();
	void draw();
	void update(float dt);
	void release();
};

