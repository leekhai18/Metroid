#pragma once
#include "BaseObject.h"
class DefendBoss: public BaseObject
{
private:
	const int* frame;
	int totalFrame;
	int heath;
	int index;
	bool beHit;
public:
	DefendBoss(TextureManager* textureM, Graphics* graphics);

	void setBeHit(bool hit);

	void decreaseHealth(float dame);

	void setBoundCollision();

	void update(float dt);

	void draw();
	DefendBoss();
	~DefendBoss();
};

