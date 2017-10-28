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
	Animation	*runningNormalAnimation,
				*runningUpAnimation,
				*runningShootAnimation,
				*rollingAnimation,
				*jumpingAnimation,
				*startingAnimation;

public:
	Samus(TextureManager* textureM, Graphics* graphics, Input* input);
	Samus();
	~Samus();

	void draw();
	void handleInput(float dt);
	void update(float dt);
	void release();

	void updateDirection();
	void running(float dt);
	void turnUp();
	void jump();

	Animation* getStartingAnim();
	Animation* getRunningNormalAnim();
	Animation* getRunningUpAnim();
	Animation* getRunningShootAnim();
	Animation* getRollingAnim();
};

