#pragma once
#include "BaseObject.h"
#include "Animation.h"

class EnergyTank : public BaseObject
{
private:
	Animation *anim;

public:
	EnergyTank();
	EnergyTank(TextureManager* textureM, Graphics* graphics);
	~EnergyTank();

	void update(float dt);
	void draw();
};

