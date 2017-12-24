#pragma once
#include "BaseObject.h"
#include <list>
class Brick : public BaseObject
{
private:
	
	float timeReset;
public:
	Brick();
	Brick(TextureManager* textureM, Graphics* graphics, BrickStyle style);
	~Brick();


	void onCollision(float dt);
	void update(float dt);
	void draw();

};

