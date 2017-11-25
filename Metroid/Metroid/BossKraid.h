#pragma once
#include "BaseObject.h"
#include "Animation.h"

class BossKraid : public BaseObject
{
private:
	Animation *anim;

public:
	BossKraid();
	BossKraid(TextureManager* textureM, Graphics* graphics);
	~BossKraid();

	void update(float dt);
	void draw();
};

