#pragma once
#include "BaseObject.h"
#include "Animation.h"

class GateBlue : public BaseObject
{
private:
	bool isHit;
	MetroidRect baseBound;
	bool isCollideSamusInPort;
	float timer;

	Animation *openAnim;
	Animation *closeAnim;

public:
	GateBlue();
	GateBlue(TextureManager* textureM, Graphics* graphics);
	~GateBlue();

	void update(float dt);
	void draw();

	void effectOpen();
	void effectClose();
	
	void setHit(bool flag);
	void setIsCollideSamusInPort(bool flag);
	void setBoundCollision(MetroidRect rect);
};

