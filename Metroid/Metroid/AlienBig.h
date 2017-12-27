#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "Brick.h"
class AlienBig : public BaseObject
{
private:
	list<Brick*>* listBrick;
	bool beHit;
public:
	AlienBig();
	AlienBig(TextureManager* textureM, Graphics* graphics);
	void setBoundCollision();
	//void setActiveBound();
	~AlienBig();
	list<Brick*>* getListBrick();
	void setBeHit();
	bool getBeHit();
	void update(float dt);
	void draw();
};

