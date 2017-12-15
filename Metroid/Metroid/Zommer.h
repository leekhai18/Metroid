#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "GameError.h"
#include "BaseObject.h"
#include "Camera.h"
#include "GameDebug.h"
#include "Collision.h"
#include <map>
using namespace std;
#define ZOMMER_COLLISION 14
#define ZOMMER_WIDTH 14
#define ZOMMER_HEIGHT 14
class Zommer: public BaseObject
{
private:
	Animation *anim;
	bool flag = true;
	bool isCollide = false;
	float x = 0;
	float y = 0;

	int health;
	list<BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;

	VECTOR2 topSide;
	VECTOR2 leftSide;
	VECTOR2 rightSide;
	VECTOR2 bottomSide;

	MetroidRect boundOfWall;
	MetroidRect positionCollide;
	ZommerDirection zommer_direction;

	bool gravity_bool[4];
	ZommerGravity gravity;

	bool isUpdate;

	bool test;
	float time;
public:
	Zommer(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Zommer();
	~Zommer();
	ZommerGravity getGravity();
	void setBoundCollision();
	void onCollision(float dt);
	void update(float dt);
	void draw();

	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

