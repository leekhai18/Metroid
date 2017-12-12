#pragma once
#include "SkreeEffectDeathPart.h"

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

	void setPosition(VECTOR2 pos);
	VECTOR2 getPosition();

	bool isFinished();
};

