#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Zeb : public BaseObject
{
private:
	Animation *anim;

	VECTOR2 samusPosition;

public:
	Zeb();
	Zeb(TextureManager* textureM, Graphics* graphics);
	~Zeb();

	void update(float dt);
	void draw();

	void init(VECTOR2 samusPosition);
};

