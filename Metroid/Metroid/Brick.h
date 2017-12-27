#pragma once
#include "BaseObject.h"
#include <list>
#include <map>
class Brick : public BaseObject
{
private:
	
	float timeReset;
	map<int,BaseObject*>* listCanCollide;
	bool visible;
public:
	Brick();
	Brick(TextureManager* textureM, Graphics* graphics, BrickStyle style);
	~Brick();
	bool getVisible();
	void setVisible(bool visible);
	void onCollision(float dt);
	void update(float dt);
	void draw();

};

