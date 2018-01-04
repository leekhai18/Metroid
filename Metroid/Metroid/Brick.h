#pragma once
#include "BaseObject.h"
#include <list>
#include <map>
class Samus;
class Brick : public BaseObject
{
private:
	float timeReset;
	map<int,BaseObject*>* listCanCollide;
	bool visible;
	int heath;
	Samus* samus;
public:
	Brick();
	Brick(TextureManager* textureM, Graphics* graphics, Samus* samus,BrickStyle style);
	~Brick();
	bool getVisible();
	void decreaseHealth(float dame);
	void setVisible(bool visible);
	void onCollision(float dt);
	void update(float dt);
	void draw();

};

