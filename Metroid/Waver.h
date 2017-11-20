#pragma once
#include "BaseObject.h"
#include "Animation.h"
#include "Camera.h"

class Waver : public BaseObject
{
private:
	Animation* anim;

	//Test
	Camera* camera;
public:
	Waver();
	Waver(TextureManager* textureM, Graphics* graphics);
	~Waver();

	void update(float dt);
	void draw();

	void setCamera(Camera *cam);
};

