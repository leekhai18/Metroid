#pragma once
#include "BaseObject.h"

class SkreeEffectDeathPart : public BaseObject
{
private:
	VECTOR2 target;
	VECTOR2 startPosition;
	float timer;
	bool isFinish;

	float d;
	float d2;
public:
	SkreeEffectDeathPart(TextureManager* textureM, Graphics* graphics);
	SkreeEffectDeathPart();
	~SkreeEffectDeathPart();

	void init(VECTOR2 target, VECTOR2 startPosition);
	void onCollision();
	void update(float dt);
	void draw();

	void setBoundCollision();

	bool isFinished();

	void finish();
};

