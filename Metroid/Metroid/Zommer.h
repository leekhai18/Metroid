#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Zommer: public BaseObject
{
private:
	Animation *anim;
	bool flag = true;
	int change = 1;

	int health;

public:
	Zommer(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Zommer();
	~Zommer();

	void update(float dt);
	void draw();
};

