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

	int durability;

public:
	GateRed();
	GateRed(TextureManager* textureM, Graphics* graphics);
	~GateRed();

	void update(float dt);
	void draw();

	void effectOpen();
	void effectClose();
	
	void setIsCollideSamusInPort(bool flag);
	void setBoundCollision(MetroidRect rect);

	int getDurability();
	void setDurability(int dura);
};

