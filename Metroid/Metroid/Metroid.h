#pragma once
#include "GameManager.h"
#include "Samus.h"
#include "Skree.h"
#include "Zeb.h"
#include "Waver.h"
#include "Rio.h"
#include "Zommer.h"
#include "Ripper.h"
#include "Text.h"
#include "Camera.h"
#include "Map.h"

class Metroid : public GameManager
{
private:
	static Metroid* instance;

	TextureManager *textureManager;
	SpriteManager *spriteManger;

	TextureManager *tileset;
	MapInfo *mapInfo;
	Map *mapBrinstar;

	// Player
	Samus *samus;
	Zommer *zommer;
	Ripper *ripper;

	Camera *camera;

	Text* fpsText;
	Text* opsText;

public:
	Metroid();
	~Metroid();

	static Metroid* getInstance();

	void initialize(HWND hwnd);
	void update(float dt);      // must override pure virtual from Game
	void handleInput(float dt);
	void collisions(float dt);  // "
	void render();      // "
	void releaseAll();
	void resetAll();

	HWND getCurrentHWND();
};

