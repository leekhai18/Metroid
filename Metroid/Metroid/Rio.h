#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "Samus.h"
#include "IFreezable.h"
#include "IExplosible.h"
#include "IBonusable.h"
class Samus;
class Rio: public BaseObject,public IFreezable,public IExplosible,public IBonusable
{
private:
	Animation *anim;

	VECTOR2 startPosition;
	VECTOR2 target;
	VECTOR2 P1;// P1 in Bézier curve
	VECTOR2 P2;// P2 in Bézier curve
	VECTOR2 P3;// P3 in Bézier curve
	VECTOR2 P4;// P4 in Bézier curve
	VECTOR2 P5;// P1 in Bézier curve

	Samus* samus;
	// Use Bézier curve
	float t;
	float t1;
	int flag = 1;
	bool start = false;
	map<int, BaseObject*>* listWallCanCollide;
	list<CollisionReturn> *listCollide;

	VECTOR2 positionBefore;
	VECTOR2 positionAfter;

	int health;
	MetroidRect startBound;
	
	bool isCollided;
	CollideDirection directCollision;
	int resetFrame;
	bool isHandle;
	bool beHit;
	float timerHit;
public:
	Rio(TextureManager* textureM, Graphics* graphics,Samus* samus, EnemyColors color);
	Rio();
	~Rio();
	void handleVelocity(float dt);
	void onCollision(float dt);
	void update(float dt);
	void draw();
	void setBoundCollision();
	bool getHandle();
	VECTOR2 getTarget();
	void setBeHit(bool hit);
	void decreaseHealth(float dame);
	void setStartPosition(VECTOR2 position);
	void setStartBound(MetroidRect rect);
	void setTarget(VECTOR2 target);

	void reInit(VECTOR2 stP);
	void reInit();

	map<int, BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

