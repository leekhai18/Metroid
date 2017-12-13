#pragma once
#include "BaseObject.h"

class Port : public BaseObject
{
private:
	bool isSetUpPosition;

public:
	Port();
	~Port();

	void update(float dt);
	void draw();
};

