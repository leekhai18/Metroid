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
#include "IExplosible.h"
#include "IBonusable.h"
#include <list>
#include "Samus.h"

class Zeb : public BaseObject,public IFreezable,public IExplosible,public IBonusable
{
private:
	Animation *anim;
	VECTOR2 samusPosition;
	VECTOR2 startPosition;
	int health;
	bool allowFly;

	bool beHit;	
	float timerHit;
	Samus* samus;
public:
	Zeb();
	Zeb(TextureManager* textureM, Graphics* graphics, EnemyColors color, Samus* samus);
	~Zeb();

	void update(float dt);
	void draw();
	void setBeHit(bool hit);
	void decreaseHealth(float dame);
	void setBoundCollision();
	void handleVelocity(float dt);
	void setStartPosition(VECTOR2 startPosition);
	void reInit();
	void setTarget(VECTOR2 samusPosition);
};

