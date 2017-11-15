#pragma once
#include "GameManager.h"
#include "Samus.h"
#include "Skree.h"
#include "Zeb.h"
#include "Waver.h"
#include "Rio.h"
#include "Zommer.h"
#include "Text.h"
#include "Camera.h"

class Metroid : public GameManager
{
private:
	static Metroid* instance;

	TextureManager *textureManager;
	SpriteManager *spriteManger;

	Samus *samus;

	Camera *camera;

	// Enemy
	Skree *skree;
	Zeb *zeb;
	Waver *waver;
	Rio *rio;
	Zommer *zommer;

	Text* fpsText;

public:
	Metroid();
	~Metroid();

	static Metroid* getInstance();

	void initialize(HWND hwnd);
	void update(float dt);      // must override pure virtual from Game
	void ai();          // "
	void collisions();  // "
	void render();      // "
	void releaseAll();
	void resetAll();

	HWND getCurrentHWND();
};

