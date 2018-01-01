#include "BossRocketPool.h"


#define NUMBER_ROCKET 5
BossRocketPool* BossRocketPool::instance=nullptr;

BossRocketPool::BossRocketPool()
{
}

void BossRocketPool::handleVelocity(float dt)
{
}

void BossRocketPool::onCollision(float dt)
{
}

void BossRocketPool::update(float dt)
{
}

void BossRocketPool::draw()
{
}

void BossRocketPool::release()
{
	for (int i = 0; i < instance->rocketList.size(); i++)
	{
		delete instance->rocketList[i];
	}
	for (int i = 0; i < instance->rocketListUsing.size(); i++)
	{
		delete instance->rocketListUsing[i];
	}
	delete instance;
}

BossRocketPool * BossRocketPool::getInstance()
{
	return instance;
}

vector<BossKraidRocket*> BossRocketPool::getListRocket()
{
	return this->rocketList;
}

vector<BossKraidRocket*> BossRocketPool::getListRocketUsing()
{
	return this->rocketListUsing;
}

BossRocketPool::BossRocketPool(Graphics * graphics, TextureManager * textureM, Samus * samus, BossKraid * boss)
{
	this->samus = samus;
	this->boss = boss;
	this->graphics = graphics;
	this->textureM = textureM;
	for (int i = 0; i < NUMBER_ROCKET; i++)
	{
		BossKraidRocket* rocket = new BossKraidRocket(graphics, textureM, samus, boss);
		this->rocketList.push_back(rocket);
	}
	instance = this;
}
BossKraidRocket * BossRocketPool::getRocket()
{
	if (this->rocketList.empty())
	{
		BossKraidRocket* rocket = new BossKraidRocket(graphics, textureM, samus, boss);


		this->rocketListUsing.push_back(rocket);
		return rocket;
	}
	else
	{
		BossKraidRocket* bullet = this->rocketList.front();

		if (bullet->isInStatus(eStatus::ENDING))
		{
			this->rocketList.erase(this->rocketList.begin());

			this->rocketListUsing.push_back(bullet);
			return bullet;
		}

		BossKraidRocket* rocket = new BossKraidRocket(graphics, textureM, samus, boss);

		this->rocketListUsing.push_back(rocket);
		return rocket;
	}
}

void BossRocketPool::returnPool(BossKraidRocket * rocket)
{
	rocket->returnPool();
	this->rocketList.push_back(rocket);

	for (unsigned i = 0; i < this->rocketListUsing.size(); i++)
	{
		if (this->rocketListUsing[i] == rocket)
			this->rocketListUsing.erase(this->rocketListUsing.begin() + i);
	}
}

BossRocketPool::~BossRocketPool()
{
}
