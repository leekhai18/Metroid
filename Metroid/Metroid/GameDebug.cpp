#include "GameDebug.h"


GameDebug* GameDebug::instance = nullptr;

void GameDebug::setVertices(MetroidRect rect)
{
	//left top edge
	VECTOR3 left_top(rect.left, rect.top, 0);

	//left bottom edge
	VECTOR3 left_bottom(rect.left, rect.bottom, 0);

	//right bottom
	VECTOR3 right_bottom(rect.right, rect.bottom, 0);

	//right top
	VECTOR3 right_top(rect.right, rect.top, 0);

	vertices[0] = left_top;
	vertices[1] = left_bottom;
	vertices[2] = right_bottom;
	vertices[3] = right_top;
	vertices[4] = left_top;
}

GameDebug * GameDebug::getInstance()
{
	if(instance == NULL)
	{
		instance = new GameDebug();
	}
	return instance;
}

void GameDebug::initialize(Graphics * graphics)
{
	this->graphics = graphics;
	this->count = count;
}

void GameDebug::setCount(int count)
{
	this->count = count;
}

void GameDebug::draw()
{
	graphics->drawLine(this->vertices, this->count);
}

void GameDebug::release()
{
	delete[] instance->vertices;
	delete instance;
}

GameDebug::GameDebug()
{
	vertices = new VECTOR3[5];
}


GameDebug::~GameDebug()
{
}
