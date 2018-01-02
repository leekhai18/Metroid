#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include <list>
#include <map>
#include "IFreezable.h"
#include "IExplosible.h"
#include "IBonusable.h"
class Samus;
class Waver : public BaseObject, public IFreezable, public IExplosible, public IBonusable
{
private:
	Animation* anim;
	Samus* samus;
	int health;
	map<int,BaseObject*>* listWallCanCollide;
	list<CollisionReturn> *listCollide;
	float velocity_frame;
	eDirection directionX;
	WaverDirectionY directionY;

	VECTOR2 startPosition;
	bool beHit;
	float timerHit;
	MetroidRect startBound;
	bool isHandle;
	int resetFrame;
public:
	Waver();
	Waver(TextureManager * textureM, Graphics * graphics, Samus * samus, EnemyColors color);
	~Waver();

	map<int, BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
	void setStartPosition(VECTOR2 position);
	void reInit();
	void setBeHit(bool hit);
	void decreaseHealth(float dame);
	void handleVelocity(float dt);
	void setStartBound(MetroidRect rect);
	void setBoundCollision();
	void update(float dt);
	void onCollision(float dt);
	void draw();
	bool getHandle();
};

