#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include <map>
#include <list>
#include "IExplosible.h"
class Samus;
class BossKraid;
class BossKraidRocket :public BaseObject,public IExplosible
{
private:
	Samus* samus;
	Animation *anim;
	BossKraid* boss;
	VECTOR2 P1;
	VECTOR2 P2;
	VECTOR2 normalize;
	float angle;

	list<CollisionReturn>* listCollide;
	map<int, BaseObject*>* listWallCanCollide;
	bool isReInit;
	bool isHandle;

	float distance;
	float distanceShoot;


	bool isCollided;
	float timer;

	float dame;
public:
	void init();
	void returnPool();
	void handleVelocity(float dt);
	void setBoundCollision();
	void setListWallCanCollide(map<int, BaseObject*>* list);
	map<int, BaseObject*>* getListWallCanCollide();
	list<CollisionReturn>* getListCollide();
	void onCollision(float dt);
	void update(float dt);
	void draw();
	BossKraidRocket();
	BossKraidRocket(Graphics* graphics, TextureManager * textureM,Samus* samus, BossKraid* boss);
	~BossKraidRocket();
};

