#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include <list>
#include "IFreezable.h"
#include "IExplosible.h"
class Waver : public BaseObject,public IFreezable, public IExplosible
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
	bool beHit;
	float timerHit;
public:
	Waver();
	Waver(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	~Waver();

	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
	void setStartPosition(VECTOR2 position);
	void reInit();
	void setBeHit(bool hit);
	void decreaseHealth(float dame);
	void handleVelocity(float dt);
	void setBoundCollision();
	void update(float dt);
	void onCollision(float dt);
	void draw();

};

