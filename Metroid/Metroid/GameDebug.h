#pragma once
#include "Samus.h"
#include <list>
class GameDebug
{
private:
	//using to draw boundCollision of object to check collision
	VECTOR3* vertices;
	//graphics to draw
	Graphics* graphics;
	//number of vetice connect to each other
	int count;
	static GameDebug* instance;
public:
	
	void setVertices(MetroidRect rect);
	static GameDebug* getInstance();
	void initialize(Graphics* graphics);
	void setCount(int count);
	void draw();
	void release();
	GameDebug();
	~GameDebug();
};

