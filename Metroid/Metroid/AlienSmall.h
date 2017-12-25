#pragma once
#include "BaseObject.h"
#include "Animation.h"

class AlienSmall : public BaseObject
{
public:
	AlienSmall();
	AlienSmall(TextureManager* textureM, Graphics* graphics);
	~AlienSmall();
	void update(float dt);
	void draw();
};

