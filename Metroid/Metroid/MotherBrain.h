#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "Samus.h"
#include "MBulletPool.h"
#include "Port.h"
#include "IExplosible.h"
class MotherBrain : public BaseObject,public IExplosible
{
private:
	Animation *anim;
	Animation* fullHeath;
	Animation* openingBoss;
	Animation* openedBoss;
	
	MBulletPool* bulletPool;

	Samus* samus;

	bool isDead;


	Port* port;
	bool beHit;
	float timerHit;
	int health;
	float timeBeforDeath;
	bool isHandle;
	VECTOR3 position;
	int cycle;
public:
	MotherBrain();
	MotherBrain(TextureManager* textureM, Graphics* graphics,Samus* samus);
	~MotherBrain();

	void initStartBulletPool(VECTOR2 position);
	void setPort(Port* port);
	void setBoundCollision();
	void reInit();
	void handleVelocity(float dt);
	void onCollision(Samus* samus,float dt);
	void update(float dt);
	
	void draw();
	void setBeHit(bool hit);
	void decreaseHealth(float dame);
};

