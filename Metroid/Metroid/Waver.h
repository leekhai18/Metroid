#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Waver : public BaseObject
{
private:
	Animation* anim;
	int health;

public:
	Waver();
	Waver(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	~Waver();

	void update(float dt);
	void draw();
};

