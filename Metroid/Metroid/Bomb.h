#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Bomb : public BaseObject
{
private:
	Animation *anim;

public:
	Bomb();
	Bomb(TextureManager* textureM, Graphics* graphics);
	~Bomb();

	void update(float dt);
	void draw();
};

