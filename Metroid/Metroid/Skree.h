#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "SkreeEffectDeath.h"
#include "Samus.h"
#include "IFreezable.h"
#include "IExplosible.h"
#include "IBonusable.h" 
class Samus;
class Skree : public BaseObject, public IFreezable,public IExplosible,public IBonusable
{
private:
	Animation *animationRotate;
	SkreeEffectDeath *effectDeath;
	Samus* samus;

	VECTOR2 target;

	float timerDeath;

	// support to reinit
	VECTOR2 initPosition;
	bool isHandle;
	

	map<int,BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;
	bool beHit;
	float timerHit;
	float health;

	MetroidRect startBound;
public:
	Skree(TextureManager* textureM, Graphics* graphics, Samus* samus, EnemyColors color);
	Skree();
	~Skree();

	bool checkCollision(Samus* sam, float dt);
	void onCollision(Samus* sam); // check collision skree's bullets with samus

	void handleVelocity(float dt);
	void onCollision(float dt);
	void update(float dt);
	void draw();
	void release();

	void setStartBound(MetroidRect rect);

	bool getHandle();
	VECTOR2 getTarget();
	void setTarget(VECTOR2 target);

	void setBoundCollision();

	// set init
	void setInitPosition(VECTOR2 pos);

	void reInit();

	void finish();

	void setBeHit(bool flag);
	void decreaseHealth(float dame);

	map<int, BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
};

