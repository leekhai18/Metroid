#pragma once
#include "BaseObject.h"
#include "Samus.h"
#include "IExplosible.h"
class Buble:public BaseObject, public IExplosible
{
private:
	Samus* samus;
	int health;

	VECTOR2 startPosition;
	VECTOR2 P1;
	VECTOR2 normalize;
	bool beHit;
	float timerHit;
	MetroidRect startBound;
	bool isHandle;

public:
	Buble(TextureManager * textureM, Graphics * graphics, Samus * samus);

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
	Buble();
	~Buble();
};

