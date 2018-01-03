#pragma once
#include "BaseObject.h"
#include "Samus.h"
#include "CanonBullet.h"
#include "Animation.h"
#include <map>
class MachineCanon:public BaseObject
{
private:
	Samus* samus;
	map<int, BaseObject*>* listWallCanCollide;
	CanonBullet* bullet;
	Animation* anim;
	int sideToFire;
	bool stop;
	float timeDelayFire;
	bool isFire;
	int index;
	CanonType type;
	const int* listFrame;
	VECTOR2 startPosition;
public:
	MachineCanon();
	void reInit();
	void setStartPosition(VECTOR2 position);

	void setBoundCollision();
	void handleVelocity(float dt);
	void onCollision(float dt);
	void update(float dt);
	void draw();
	MachineCanon(Graphics* graphics,TextureManager* textureM,Samus* samus, CanonType type);
	~MachineCanon();
};
