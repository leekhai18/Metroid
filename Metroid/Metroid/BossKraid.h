#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "Samus.h"
#include <list>
#include "IExplosible.h"
#include "BossRocketPool.h"
class BossKraid : public BaseObject,public IExplosible
{
private:
	Animation *anim;
	Samus* samus;
	VECTOR2 P1;
	VECTOR2 P2;
	VECTOR2 P3;
	map<int,BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;
	float timeToFire;
	VECTOR2 beforePosition;
	VECTOR2 afterPosition;
	VECTOR2 startPosition;
	float floor;

	float timeJump;
	float timeRun;

	BossRocketPool* rocketPool;
	BossKraidRocket* rocketBoss;
	BossKraidRocket* rocket1;
	int health;
	bool beHit;
	float timerHit;
	bool init;

	bool stopRun;
	
public:
	BossKraid();


	BossKraid(TextureManager* textureM, Graphics* graphics, Samus* samus);
	~BossKraid();

	map<int, BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
	void setStartPosition(VECTOR2 position);
	void setBoundCollision();
	void reInit();

	void calculateBezier();
	void runToSamus();
	void lauchRocket();
	void update(float dt);
	void handleVelocity(float dt);
	void onCollision(float dt);

	

	void draw();
	void setBeHit(bool hit);
	void decreaseHealth(float dame);
};

