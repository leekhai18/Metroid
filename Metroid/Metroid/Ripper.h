#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include <list>
class Ripper : public BaseObject
{
private:
	list<BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;
public:
	Ripper();
	Ripper(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	~Ripper();

	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
	void update(float dt);
	void draw();
};

