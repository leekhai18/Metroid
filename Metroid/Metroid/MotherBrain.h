#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "Samus.h"
#include "MBulletPool.h"

class MotherBrain : public BaseObject
{
private:
	Animation *anim;

	MBulletPool* bulletPool;

	Samus* samus;

	bool isDead;

public:
	MotherBrain();
	MotherBrain(TextureManager* textureM, Graphics* graphics,Samus* samus);
	~MotherBrain();

	void initStartBulletPool(VECTOR2 position);

	void setBoundCollision();
	void reInit();
	void handleVelocity(float dt);
	void onCollision(Samus* samus,float dt);
	void update(float dt);
	
	void draw();
};

