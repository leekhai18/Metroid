#pragma once
#include "SkreeEffectDeathPart.h"

#define NUM_PART 4
#define DISTANCE 70

class SkreeEffectDeath
{
private:
	SkreeEffectDeathPart** list;

	VECTOR2 position;

public:
	SkreeEffectDeath(TextureManager* textureM, Graphics* graphics);
	SkreeEffectDeath();
	~SkreeEffectDeath();

	void update(float dt);
	void draw();

	void init(VECTOR2 position);
	bool isInit();
};

