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
#include <list>
#define RIPPER_COLLISION 14
#define RIPPER_WIDTH 14
#define RIPPER_HEIGHT 14
class Ripper : public BaseObject,public IFreezable
{
private:
	map<int,BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;

	int resetFrame;

	bool isHandle;

public:
	Ripper();
	Ripper(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	~Ripper();
	void reInit();
	void handleVelocity(float dt);
	void setBoundCollision();
	void onCollision(float dt);
	void update(float dt);
	void draw();
	map<int,BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

