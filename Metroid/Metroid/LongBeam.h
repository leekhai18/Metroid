#pragma once
#include "BaseObject.h"
#include "Animation.h"

class LongBeam : public BaseObject
{
private:
	Animation *anim;

public:
	LongBeam();
	LongBeam(TextureManager* textureM, Graphics* graphics);
	~LongBeam();

	void update(float dt);
	void draw();
};

