#pragma once
#include "BaseObject.h"
#include <list>
class Brick : public BaseObject
{
private:
	
	float timeReset;
	list<BaseObject*>* listCanCollide;
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

