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
#include <list>

#define SAMUS_MIN_SPEED_Y 70
#define SAMUS_VERLOCITY_X 100
#define SAMUS_MAX_SPEED_Y 150

#define MIN_JUMP  32.0f
#define MAX_JUMP 80.0f

#define TIME_SHOOTING 0.1f

#define MAX_HEIHT 32

#define WIDTH_RUN 20
#define HEIGHT_RUN 32

#define HEIGHT_JUMP 25
#define WIDTH_JUMP 18

#define ROLL_WIDTH 12
#define ROLL_HEIGHT 12

#define JUMP_HEIGHT 24
#define JUMP_WIDTH 10

#define ACCELERATE_Y -140.0f
#define ACCELERATE_X 50.0f

#define OFFSET_JUMP 4
#define OFFSET_RUN 5
#define OFFSET_UP 5
#define OFFSET_ACROBAT 2

#define TIME_TO_ACROBAT 0.1f

#define OFFSET_STAND 19
#define WIDTH_COLLISION 10
#define HEIGHT_COLLISION 30
#define OFFSET_COLLISION_X 0
#define OFFSET_COLLISION_Y 4

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

	list<CollisionReturn> *listCollide;

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

	void onCollision();

	void setIsCollidingPort(bool flag);
	bool isColliedPort();
	void setCanMoveToFrontGate(bool flag);

	Animation* getStartingAnim();
	Animation* getRunningNormalAnim();
	Animation* getRunningUpAnim();
	Animation* getRunningHittingRightAnim();
	Animation* getRollingAnim();
	Animation* getJumpingAnim();

	list<CollisionReturn> *getListCollide();
};

