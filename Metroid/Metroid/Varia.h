#pragma once
#include "BaseObject.h"
#include "Animation.h"

class Varia : public BaseObject
{
private:
	Animation *anim;

public:
	Varia();
	Varia(TextureManager* textureM, Graphics* graphics);
	~Varia();

	void update(float dt);
	void draw();

	void setActivity(bool flag);
	bool getActivity();
};

