#pragma once
#include "BaseObject.h"
#include "Animation.h"

class MaruMari : public BaseObject
{
private:
	Animation *anim;

	bool isActivity;

public:
	MaruMari();
	MaruMari(TextureManager* textureM, Graphics* graphics);
	~MaruMari();

	void update(float dt);
	void draw();

	void setActivity(bool flag);
	bool getActivity();
};

