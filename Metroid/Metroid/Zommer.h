#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Zommer: public BaseObject
{
private:
	Animation *anim;
	bool flag = true;
	int change = 1;

public:
	Zommer(TextureManager* textureM, Graphics* graphics);
	Zommer();
	~Zommer();

	void update(float dt);
	void draw();
};

