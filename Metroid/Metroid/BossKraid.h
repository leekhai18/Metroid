#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "Samus.h"
#include <list>
class BossKraid : public BaseObject
{
private:
	Animation *anim;
	Samus* samus;
	VECTOR2 P1;
	VECTOR2 P2;
	VECTOR2 P3;
	list<BaseObject*> *listWallCanCollide;
	list<CollisionReturn> *listCollide;
	float time;
	bool isDown;
	bool isActivity;
	VECTOR2 beforePosition;
	VECTOR2 afterPosition;
	VECTOR2 startPosition;
	float floor;
public:
	BossKraid();


	BossKraid(TextureManager* textureM, Graphics* graphics, Samus* samus);
	~BossKraid();

	list<BaseObject*>* getListWallCanCollide();
	list<CollisionReturn> *getListCollide();
	void setStartPosition(VECTOR2 position);
	void setBoundCollision();
	void reInit();
	void calculateBezier();
	void update(float dt);
	void handleVelocity(float dt);
	void onCollision(float dt);
	
	void draw();
};

