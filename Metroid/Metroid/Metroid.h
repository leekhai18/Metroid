#pragma once
#include "GameManager.h"
#include "Samus.h"
#include "Skree.h"
#include "Zeb.h"
#include "Waver.h"
#include "Rio.h"
#include "Zommer.h"

class Metroid : public GameManager
{
private:
	Samus *samus;
	TextureManager *textureManager;
	SpriteManager *spriteManger;

	// Enemy
	Skree *skree;
	Zeb *zeb;
	Waver *waver;
	Rio *rio;
	Zommer *zommer;

public:
	Metroid();
	~Metroid();


	void initialize(HWND hwnd);
	void update(float dt);      // must override pure virtual from Game
	void ai();          // "
	void collisions();  // "
	void render();      // "
	void releaseAll();
	void resetAll();
};

