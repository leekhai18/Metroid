#pragma once
#include "BaseObject.h"
#include "Animation.h"

class BossDragon : public BaseObject
{
public:
	BossDragon();
	BossDragon(TextureManager* textureM, Graphics* graphics);
	~BossDragon();

	void update(float dt);
	void draw();
};

