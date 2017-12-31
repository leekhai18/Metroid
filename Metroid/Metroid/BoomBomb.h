#pragma once
#include "BaseObject.h"
#include <list>
#include "Animation.h"
class Samus;

class BoomBomb : public BaseObject
{
private:
	Animation* anim;
	Animation* explosion;
	list<BaseObject*> *listCollide;

	float timer;

	float dame;

	bool canHandledCollision;

	Samus* samus;
public:
	BoomBomb(TextureManager* textureM, Graphics* graphics,Samus* samus);
	void setSamus(Samus* samus);
	BoomBomb();
	~BoomBomb();

	void update(float dt);
	void draw();

	void onCollision();

	void setBoundCollision();

	void start(VECTOR2 pos);

	// Use object pool
	void returnPool();

	list<BaseObject*> *getListCollide();
};

