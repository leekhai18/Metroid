#pragma once
#include <iostream>
#include <vector>
#include "BoomBomb.h"
class Samus;
class BoomBombPool
{
private:
	TextureManager* textureManager;
	Graphics* graphics;

	std::vector<BoomBomb*> list;
	std::vector<BoomBomb*> listUsing;
	Samus* samus;
	static BoomBombPool* instance;
public:
	static BoomBombPool* getInstance();
	void setSamus(Samus* samus);
	BoomBombPool(TextureManager* textureM, Graphics* graphics, Samus* samus);
	BoomBombPool(TextureManager* textureM,Graphics* graphics, Samus* samus, int startNumberElement);
	~BoomBombPool();

	BoomBomb* getBoom();
	void returnPool(BoomBomb* bom);

	std::vector<BoomBomb*> getListUsing();

	void release();
};
