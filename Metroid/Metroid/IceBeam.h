#pragma once
#include "BaseObject.h"
#include "Animation.h"

class IceBeam : public BaseObject
{
private:
	Animation *anim;

public:
	IceBeam();
	IceBeam(TextureManager* textureM, Graphics* graphics);
	~IceBeam();

	void update(float dt);
	void draw();

	void setActivity(bool flag);
	bool getActivity();
};

