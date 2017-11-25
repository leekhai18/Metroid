#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Waver : public BaseObject
{
private:
	Animation* anim;

public:
	Waver();
	Waver(TextureManager* textureM, Graphics* graphics);
	~Waver();

	void update(float dt);
	void draw();
};

