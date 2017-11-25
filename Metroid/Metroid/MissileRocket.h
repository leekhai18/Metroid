#pragma once
#include "BaseObject.h"
#include "Animation.h"

class MissileRocket : public BaseObject
{
private:
	Animation *anim;

public:
	MissileRocket();
	MissileRocket(TextureManager* textureM, Graphics* graphics);
	~MissileRocket();

	void update(float dt);
	void draw();
};

