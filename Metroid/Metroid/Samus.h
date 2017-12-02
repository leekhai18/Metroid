#pragma once
#include "Sprite.h"
#include "Graphics.h"
#include "Input.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "GameError.h"
#include "Animation.h"
#include "BaseObject.h"
#include "BulletPool.h"

#define SAMUS_HEIGHT 30
#define SAMUS_VERLOCITY_X 80
#define SAMUS_VERLOCITY_Y 80
#define MIN_JUMP  32.0f
#define MAX_JUMP 80.0f
#define TIME_SHOOTING 0.15f
#define MAX_WIDTH 12
#define MAX_HEIHT 32
#define WIDTH_RUN 24
#define HEIGHT_RUN 32
#define ROLL_WIDTH 12
#define ROLL_HEIGHT 12

// In tilemap
#define POSITION_FIRST_X 632
#define POSITION_FIRST_Y 3280

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
	bool acrobat;
	bool moveRight;
	bool moveLeft;
	bool jump;
	bool moveHorizontal;
	bool moveVertical;
	float totalHeightWasJumped;

	bool isCollidingPort;
	bool moveToFontGate;
	float distance;

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
	bool isAcrobat();
	bool canMoveLeft();
	bool canMoveRight();
	bool isJump();

	void setJump(bool jump);
	void setCanMoveLeft(bool moveLeft);
	void setCanMoveRight(bool moveRight);
	void setFall(bool isFall);
	void setAcrobat(bool acrobat);

	void setBoundCollision(MetroidRect rect);
	void setActiveBound();

	void onCollision(BaseObject* object, CollisionReturn result);

	void setIsCollidingPort(bool flag);
	bool isColliedPort();
	void setCanMoveToFrontGate(bool flag);

	Animation* getStartingAnim();
	Animation* getRunningNormalAnim();
	Animation* getRunningUpAnim();
	Animation* getRunningHittingRightAnim();
	Animation* getRollingAnim();
	Animation* getJumpingAnim();
};

