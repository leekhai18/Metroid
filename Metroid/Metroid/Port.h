#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Port : public BaseObject
{
public:
	Port();
	~Port();

	void update(float dt);
	void draw();
};

