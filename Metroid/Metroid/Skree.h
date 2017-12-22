#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "SkreeEffectDeath.h"
#include "Samus.h"
#include "IFreezable.h"

class Skree : public BaseObject, public IFreezable
{
private:
	Animation *animationRotate;
	SkreeEffectDeath *effectDeath;
	Animation *explosion;

	VECTOR2 target;

	float timerDeath;

	// support to reinit
	VECTOR2 initPosition;
	bool canDraw;

	list<BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;
	bool beHit;
	float timerHit;
	float health;

public:
	Skree(TextureManager* textureM, Graphics* graphics, EnemyColors color);
	Skree();
	~Skree();

	bool checkCollision(Samus* sam, float dt);
	void onCollision(Samus* sam); // check collision skree's bullets with samus

	void onCollision(float dt);
	void update(float dt);
	void draw();
	void release();

	VECTOR2 getTarget();
	void setTarget(VECTOR2 target);

	void setBoundCollision();

	// set init
	void setInitPosition(VECTOR2 pos);

	void reInit();

	void finish();

	void setBeHit(bool flag);
	void decreaseHealth(float dame);

	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

