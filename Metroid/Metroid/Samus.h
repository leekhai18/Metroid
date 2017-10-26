#pragma once
#include "Sprite.h"
#include "Graphics.h"
#include "Input.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "GameError.h"
#include "Animation.h"
#include "BaseObject.h"

#define SAMUS_VERLOCITY 100



class Samus: public BaseObject
{
private:
	Input* input;
	Animation	*runningAnimation,
				*rollingAnimation,
				*jumpingAnimation,
				*startingAnimation;

public:
	Samus(TextureManager* textureM, Graphics* graphics, Input* input);
	Samus();
	~Samus();

	void setRect();
	void draw();
	void handleInput(float dt);
	void update(float dt);
	void release();

	void updateDirection();
	void running(float dt);
	void turnUp();
	void jump();

	Animation* getStartingAnim();
};

