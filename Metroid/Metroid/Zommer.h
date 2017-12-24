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
using namespace std;
#define ZOMMER_COLLISION 14
#define ZOMMER_WIDTH 14
#define ZOMMER_HEIGHT 14
class Zommer : public BaseObject,public IFreezable ,public IExplosible
{
private:
	Animation *anim;
	bool isCollide = false;

	
	list<BaseObject*> *listWallCanCollide;
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
public:
	Zommer(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Zommer();
	~Zommer();
	ZommerGravity getGravity();
	void reInit();
	void handleVelocity(float dt);
	void setBoundCollision();
	void setStartBound(MetroidRect rect);
	void onCollision(float dt);
	void update(float dt);
	void draw();
	void setStartPosition(VECTOR2 position);
	void setBeHit(bool hit);

	void decreaseHealth(float dame);

	//void setActiveBound()
	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

