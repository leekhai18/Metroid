#pragma once
#include "BaseObject.h"

class GateBlue : public BaseObject
{
private:
	bool isHit;
	float timer;
	MetroidRect baseBound;
	bool isCollideSamusInPort;

public:
	GateBlue();
	GateBlue(TextureManager* textureM, Graphics* graphics);
	~GateBlue();

	void update(float dt);
	void draw();

	void effectDisappear();
	void effectRespawn();
	
	void setHit(bool flag);
	void setIsCollideSamusInPort(bool flag);
	void setBoundCollision(MetroidRect rect);
};

