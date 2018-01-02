#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "GameError.h"
#include "BaseObject.h"
#include "Camera.h"
#include "GameDebug.h"
#include "Collision.h"
#include <map>
#include "IFreezable.h"
#include "IExplosible.h"
#include "IBonusable.h"
using namespace std;
#define ZOMMER_COLLISION 14
#define ZOMMER_WIDTH 14
#define ZOMMER_HEIGHT 14
class Samus;
class Zommer : public BaseObject,public IFreezable ,public IExplosible,public IBonusable
{
private:
	Animation *anim;
	bool isCollide = false;
	
	Samus* samus;
	map<int,BaseObject*>* listWallCanCollide;
	list<CollisionReturn> *listCollide;

	MetroidRect positionCollide;
	ZommerDirection zommer_direction;

	bool gravity_bool[4];
	ZommerGravity gravity;

	bool isUpdate;
	bool beHit;
	float timerHit;
	int health;

	VECTOR2 startPosition;
	MetroidRect startBound;
	bool isHandle;

	int reset;
	EnemyColors color;
public:
	Zommer(TextureManager* textureM, Graphics* graphics, Samus* samus, EnemyColors color);
	Zommer();
	~Zommer();
	ZommerGravity getGravity();
	void reInit();
	bool getHandle();
	void handleVelocity(float dt);
	void setBoundCollision();
	void setStartBound(MetroidRect rect);
	void onCollision(float dt);
	void update(float dt);
	void draw();
	void setStartPosition(VECTOR2 position);
	void setBeHit(bool hit);

	void decreaseHealth(float dame);

	//void setActiveBound();
	map<int, BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

