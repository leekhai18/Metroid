#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include <list>
class Waver : public BaseObject
{
private:
	Animation* anim;
	int health;
	list<BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;
	float velocity_frame;
	eDirection directionX;
	WaverDirectionY directionY;

	VECTOR2 startPosition;
	bool active;
public:
	Waver();
	Waver(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	~Waver();

	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
	void setStartPosition(VECTOR2 position);
	void setBoundCollision();
	void update(float dt);
	void onCollision(float dt);
	void draw();

};

