#pragma once
#include "BaseObject.h"
#include "Animation.h"

enum BrickStyle
{
	BrickSecretBlue,
	BrickSerectGreen,
	BrickGreen,
	BrickBlue,
	BrickRed
};

class Brick : public BaseObject
{
private:
	bool isVisible;

public:
	Brick();
	Brick(TextureManager* textureM, Graphics* graphics, BrickStyle style);
	~Brick();

	void update(float dt);
	void draw();

	void setVisible(bool visible);
};

