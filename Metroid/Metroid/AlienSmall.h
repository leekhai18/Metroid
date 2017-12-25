#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "AlienBig.h"
class AlienSmall : public BaseObject
{
private:
	AlienBig* big;
	bool beHit;
public:
	AlienSmall();
	AlienSmall(TextureManager* textureM, Graphics* graphics,AlienBig* big);
	~AlienSmall();
	void setBeHit();
	void setBoundCollision();
	void update(float dt);
	void draw();
};

