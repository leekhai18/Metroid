#pragma once
#include "BaseObject.h"
#include "Animation.h"


#define TIME_FRAME_DELAY 0.15f
#define X_OVER_HEAD 200
#define Y_OVER_TARGET 2
#define RATE_AREA_ACTIVE 0.15f
#define WIDTH_AREA_ACTIVE 200

class Rio: public BaseObject
{
private:
	Animation *animation;

	VECTOR2 startPosition;
	VECTOR2 target;
	VECTOR2 P1;// P1 in Bézier curve
	VECTOR2 P2;// P2 in Bézier curve
	VECTOR2 P3;// P3 in Bézier curve
	VECTOR2 P4;// P4 in Bézier curve
	VECTOR2 P5;// P1 in Bézier curve


	// Use Bézier curve
	float t;
	float t1;
	int flag = 0;
	
	bool statusRollTarget;
public:
	Rio(TextureManager* textureM, Graphics* graphics);
	Rio();
	~Rio();

	void update(float dt);
	void draw();
	void release();

	VECTOR2 getTarget();
	bool getStatusRollTarget();
	void setTarget(VECTOR2 target, bool statusTarget);
};

