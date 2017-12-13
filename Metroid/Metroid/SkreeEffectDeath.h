#pragma once
#include "SkreeEffectDeathPart.h"
#include "Samus.h"

class SkreeEffectDeath
{
private:
	SkreeEffectDeathPart** list;
	VECTOR2 position;

public:
	SkreeEffectDeath(TextureManager* textureM, Graphics* graphics);
	SkreeEffectDeath();
	~SkreeEffectDeath();

	bool checkCollision(Samus* sam, float dt);

	void update(float dt);
	void draw();

	void init(VECTOR2 position);

	void setPosition(VECTOR2 pos);
	VECTOR2 getPosition();

	bool isFinished();
};

