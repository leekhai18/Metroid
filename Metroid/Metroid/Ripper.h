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
	list<BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;
public:
	Ripper();
	Ripper(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	~Ripper();

	void setBoundCollision();
	void onCollision(float dt);
	void update(float dt);
	void draw();
	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

