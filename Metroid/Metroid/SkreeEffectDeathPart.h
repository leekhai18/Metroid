#pragma once
#include "BaseObject.h"
#include "Samus.h"

class SkreeEffectDeathPart : public BaseObject
{
private:
	VECTOR2 target;
	VECTOR2 startPosition;
	float timer;
	bool isFinish;

public:
	SkreeEffectDeathPart(TextureManager* textureM, Graphics* graphics);
	SkreeEffectDeathPart();
	~SkreeEffectDeathPart();

	void init(VECTOR2 target, VECTOR2 startPosition);


	bool checkCollision(Samus* sam, float dt);

	void update(float dt);
	void draw();

	void setBoundCollision();
	MetroidRect getBoundCollision();


	bool isFinished();

	void finish();
};

