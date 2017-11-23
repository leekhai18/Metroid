#pragma once
#include "BaseObject.h"
#include "Animation.h"

class GateBlue : public BaseObject
{
public:
	GateBlue();
	GateBlue(TextureManager* textureM, Graphics* graphics);
	~GateBlue();

	void update(float dt);
	void draw();
};

