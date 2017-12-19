#pragma once
#include "BaseObject.h"
#include "Animation.h"

class GateRed : public BaseObject
{
private:
	bool isHit;
	bool isCollideSamusInPort;
	float timer;

	Animation *openAnim;
	Animation *closeAnim;

public:
	GateRed();
	GateRed(TextureManager* textureM, Graphics* graphics);
	~GateRed();

	void update(float dt);
	void draw();

	void effectOpen();
	void effectClose();
	
	void setHit(bool flag);
	void setIsCollideSamusInPort(bool flag);
	void setBoundCollision(MetroidRect rect);
};

