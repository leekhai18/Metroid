#pragma once
#include <iostream>
#include <vector>
#include "BoomBomb.h"

class BoomBombPool
{
private:
	TextureManager* textureManager;
	Graphics* graphics;

	std::vector<BoomBomb*> list;
	std::vector<BoomBomb*> listUsing;

	static BoomBombPool* instance;
public:
	static BoomBombPool* getInstance();

	BoomBombPool(TextureManager* textureM, Graphics* graphics);
	BoomBombPool(TextureManager* textureM, Graphics* graphics, int startNumberElement);
	~BoomBombPool();

	BoomBomb* getBoom();
	void returnPool(BoomBomb* bom);

	std::vector<BoomBomb*> getListUsing();

	void release();
};
