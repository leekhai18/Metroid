#pragma once
#include "MotherBrainBullet.h"
#include <map>
#include "Samus.h"
class MBulletPool
{
private:
	
	VECTOR2 ownPosition;

	VECTOR2 targetPosition;

	VECTOR2 middlePosition;


	VECTOR2 distance;

	VECTOR2 beginPosition;
	VECTOR2 finishPosition;
	Samus* samus;

	map<int, MotherBrainBullet*>* bulletPool;

	int index;

	bool  finish;

	bool isCreateBullets;

	float timeDelay;

	int count;

	int numberBullet;

	float time;
public:
	

	MBulletPool(TextureManager * textureM, Graphics * graphics, Samus* samus, int numberBullet);

	void setOwnPosition(VECTOR2 position);

	void handleVelocity(float dt);

	void onCollision(float dt);

	void update(float dt);

	void draw();

	void reInit();

	MBulletPool();

	~MBulletPool();
};

