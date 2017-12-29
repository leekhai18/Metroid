#pragma once
#include "BaseObject.h"

class Port : public BaseObject
{
private:
	bool isSetUpPosition;
	bool isReset;

	bool isWin;
public:
	Port();
	~Port();
	bool Win();
	void setWin(bool win);
	void update(float dt);
	void draw();
};

