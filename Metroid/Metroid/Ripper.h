#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Ripper : public BaseObject
{
public:
	Ripper();
	Ripper(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	~Ripper();

	void update(float dt);
	void draw();
};

