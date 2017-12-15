#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "GameError.h"
#include "BaseObject.h"
#include "Camera.h"
#include "GameDebug.h"
#include "Collision.h"

class Zommer: public BaseObject
{
private:
	Animation *anim;
	bool flag = true;
	bool isCollide = false;
	float x = 0;
	float y = 0;
	MetroidRect bound;
	int direction;
	int health;
	list<BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;

public:
	Zommer(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Zommer();
	~Zommer();

	void setBoundCollision();
	void onCollision(float dt);
	void update(float dt);
	void draw();

	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

