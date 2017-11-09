#pragma once
#include "Sprite.h"
#include "Graphics.h"
#include "Input.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "GameError.h"
#include "Animation.h"
#include "BaseObject.h"
#include "Bullet.h"
#include "BulletPool.h"

#define SAMUS_WIDTH_UP_HALF 7
#define SAMUS_WIDTH_RL_HALF 10
#define SAMUS_HEIGHT 30
#define SAMUS_VERLOCITY_X 100
#define SAMUS_VERLOCITY_Y 100
#define MAX_HEIGHT_CAN_JUMP (GAME_HEIGHT * 0.5)
#define TIME_SHOOTING 0.1f


class Samus: public BaseObject
{
private:
	Input* input;

	Animation	*runningNormalAnimation,
				*runningUpAnimation,
				*runningHittingRightAnimation,
				*rollingAnimation,
				*jumpingAnimation,
				*startingAnimation;

	bool isFalling;
	float totalHeightWasJumped;

	BulletPool *bulletPool;

public:
	float timerShoot;

	Samus(TextureManager* textureM, Graphics* graphics, Input* input);
	Samus();
	~Samus();

	void draw();
	void handleInput(float dt);
	void update(float dt);
	void release();

	void updateHorizontal(float dt);
	void updateVertical(float dt);
	
	bool isFaling();
	void setFall(bool isFall);

	Animation* getStartingAnim();
	Animation* getRunningNormalAnim();
	Animation* getRunningUpAnim();
	Animation* getRunningHittingRightAnim();
	Animation* getRollingAnim();
	Animation* getJumpingAnim();

	std::vector<Bullet*> getListBulletUsing();
};

