#pragma once
#include "BaseObject.h"
#include "Animation.h"

class EnergyTank : public BaseObject
{
private:
	Animation *anim;

	bool isActivity;

public:
	EnergyTank();
	EnergyTank(TextureManager* textureM, Graphics* graphics);
	~EnergyTank();

	void update(float dt);
	void draw();

	void setActivity(bool flag);
	bool getActivity();
};

