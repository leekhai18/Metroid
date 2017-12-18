#pragma once
#include "BaseObject.h"
#include "Animation.h"

class MotherBrain : public BaseObject
{
public:
	MotherBrain();
	MotherBrain(TextureManager* textureM, Graphics* graphics);
	~MotherBrain();

	void update(float dt);
	void draw();
};

