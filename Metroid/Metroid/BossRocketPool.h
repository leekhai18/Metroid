#pragma once
#include "BossKraidRocket.h"
#include <map>
#include <vector>
class Samus;
class BossKraid;
class BossRocketPool
{
private:
	vector<BossKraidRocket*> rocketList;

	vector <BossKraidRocket*> rocketListUsing;
	TextureManager* textureM;
	Graphics* graphics;
	Samus* samus;
	BossKraid* boss;

	static BossRocketPool* instance;
public:
	BossRocketPool();

	void handleVelocity(float dt);

	void onCollision(float dt);

	void update(float dt);

	void draw();


	void release();

	static BossRocketPool* getInstance();

	vector<BossKraidRocket*> getListRocket();

	vector <BossKraidRocket*> getListRocketUsing();

	BossRocketPool(Graphics* graphics,TextureManager* textureM,Samus* samus, BossKraid* boss);

	BossKraidRocket * getRocket();

	void returnPool(BossKraidRocket * rocket);

	~BossRocketPool();
};

