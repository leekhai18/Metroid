#pragma once
#include "BaseObject.h"
class CanonBullet:public BaseObject
{
public:
	CanonBullet();
	CanonBullet(Graphics* graphics, TextureManager* textureM);
	void init();
	void end();
	void setBoundCollision();
	void handleVelocity(float dt);
	void onCollision(float dt);
	void update(float dt);
	void draw();
	~CanonBullet();
};

