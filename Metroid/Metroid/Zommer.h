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
using namespace std;
#define ZOMMER_COLLISION 14
#define ZOMMER_WIDTH 14
#define ZOMMER_HEIGHT 14
class Zommer : public BaseObject,public IFreezable
{
private:
	Animation *anim;
	bool isCollide = false;

	int health;
	list<BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;

	MetroidRect positionCollide;
	ZommerDirection zommer_direction;

	bool gravity_bool[4];
	ZommerGravity gravity;

	bool isUpdate;


	bool beHit;
	float timerHit;
public:
	Zommer(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Zommer();
	~Zommer();
	ZommerGravity getGravity();
	void setBoundCollision();
	void onCollision(float dt);
	void update(float dt);
	void draw();
	

	//void setActiveBound()
	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

