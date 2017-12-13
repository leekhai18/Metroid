#pragma once
#include "BaseObject.h"
#include "Animation.h"

class MaruMari : public BaseObject
{
private:
	Animation *anim;
	Animation *explosion;

public:
	MaruMari();
	MaruMari(TextureManager* textureM, Graphics* graphics);
	~MaruMari();

	void update(float dt);
	void draw();

	void startExplosion();
};

