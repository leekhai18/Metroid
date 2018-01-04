#pragma once
#include "BaseObject.h"
#include <map>
#include <list>
#include "Animation.h"
#include "Samus.h"
class MachineCanon;
class CanonBullet:public BaseObject
{
private:
	CanonType type;
	MachineCanon* canon;
	Animation* explose;
	VECTOR2 P1;
	VECTOR2 P2;
	VECTOR2 normalize;
	map<int, BaseObject*>* listWallCanCollide;
	list<CollisionReturn>* listCollide;
	bool isCollided;
	float timeHit;
	int resetFrame;
	int dame;
public:
	CanonBullet();
	CanonBullet(Graphics* graphics, TextureManager* textureM,CanonType type, MachineCanon* canon);
	void setListWallCanCollide(map<int, BaseObject*>* list);
	void init();
	void end();
	bool getCollided();
	void setCollided(bool collide);
	void onCollisionSamus(Samus* samus,float dt);
	list<CollisionReturn>* getListCollide( );
	void setBoundCollision();
	void handleVelocity(float dt);
	void onCollision(float dt);
	void update(float dt);
	void draw();
	~CanonBullet();
};

