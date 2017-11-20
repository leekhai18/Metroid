#pragma once
#include "BaseObject.h"

class SkreeEffectDeathPart : public BaseObject
{
private:
	VECTOR2 target;
	VECTOR2 startPosition;

	// Use Bézier curve
	float t;

public:
	SkreeEffectDeathPart(TextureManager* textureM, Graphics* graphics);
	SkreeEffectDeathPart();
	~SkreeEffectDeathPart();

	void init(VECTOR2 target, VECTOR2 startPosition);
	void update(float dt);
	void draw();
	void release();
};

