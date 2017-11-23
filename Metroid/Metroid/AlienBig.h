#pragma once
#include "BaseObject.h"
#include "Animation.h"

class AlienBig : public BaseObject
{
public:
	AlienBig();
	AlienBig(TextureManager* textureM, Graphics* graphics);
	~AlienBig();

	void update(float dt);
	void draw();
};

