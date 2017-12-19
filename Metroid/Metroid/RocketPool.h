#pragma once
#include <iostream>
#include <vector>
#include "Rocket.h"

class RocketPool
{
private:
	TextureManager* textureManager;
	Graphics* graphics;

	std::vector<Rocket*> list;
	std::vector<Rocket*> listUsing;

	static RocketPool* instance;
public:
	static RocketPool* getInstance();

	RocketPool(TextureManager* textureM, Graphics* graphics);
	RocketPool(TextureManager* textureM, Graphics* graphics, int startNumberElement);
	~RocketPool();

	Rocket* getRocket();
	void returnPool(Rocket* bullet);

	std::vector<Rocket*> getListUsing();

	void release();
};
