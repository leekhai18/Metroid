#include "BulletPool.h"

BulletPool* BulletPool::instance = nullptr;

BulletPool * BulletPool::getInstance()
{
	return instance;
}

BulletPool::BulletPool(TextureManager* textureM, Graphics* graphics)
{
	this->textureManager = textureM;
	this->graphics = graphics;

	instance = this;
}

BulletPool::BulletPool(TextureManager * textureM, Graphics * graphics, int startNumberElement)
{
	this->textureManager = textureM; 
	this->graphics = graphics;

	for (int i = 0; i < startNumberElement; i++)
	{
		this->list.push_back(new Bullet(textureM, graphics));
	}

	instance = this;
}

BulletPool::~BulletPool()
{
}

Bullet * BulletPool::getBullet()
{
	if (this->list.empty())
	{
		return new Bullet(textureManager, graphics);
	}
	else
	{
		Bullet* bullet = this->list.front();

		if (bullet->isInStatus(eStatus::ENDING))
		{
			this->list.erase(this->list.begin());

			this->listUsing.push_back(bullet);
			return bullet;
		}	

		Bullet* bul = new Bullet(textureManager, graphics);
		this->listUsing.push_back(bul);
		return bul;
	}
}

void BulletPool::returnPool(Bullet * bullet)
{
	bullet->returnPool();
	this->list.push_back(bullet);
	
	for (unsigned i = 0; i < this->listUsing.size(); i++)
	{
		if (this->listUsing[i] == bullet)
			this->listUsing.erase(this->listUsing.begin() + i);
	}
}

std::vector<Bullet*> BulletPool::getListUsing()
{
	return this->listUsing;
}

void BulletPool::release()
{
	while (this->list.size())
	{
		Bullet *obj = this->list.front();
		this->list.erase(this->list.begin());
		delete obj;
	}

	delete instance;
}
