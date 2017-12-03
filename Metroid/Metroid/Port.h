#pragma once
#include "BaseObject.h"

class Port : public BaseObject
{
public:
	Port();
	~Port();

	void update(float dt);
	void draw();
};

