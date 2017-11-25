#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Zeb : public BaseObject
{
private:
	Animation *anim;
	VECTOR2 samusPosition;

	int health;

public:
	Zeb();
	Zeb(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	~Zeb();

	void update(float dt);
	void draw();

	void init(VECTOR2 samusPosition);
};

