#pragma once
#include "constants.h"
#include "MapInfo.h"
#include "textureManager.h"
#include "graphics.h"

class Map
{
private:
	MapInfo* info;
	TextureManager* texture;
	Graphics* graphics;

public:
	Map();
	~Map();

	bool initialize(Graphics* graphics, TextureManager* texture, MapInfo* info);

	void draw();
	void update(float dt);
	void collision();
};

