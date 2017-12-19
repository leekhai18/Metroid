#include "BoomBombPool.h"

BoomBombPool* BoomBombPool::instance = nullptr;

BoomBombPool * BoomBombPool::getInstance()
{
	return instance;
}

BoomBombPool::BoomBombPool(TextureManager* textureM, Graphics* graphics)
{
	this->textureManager = textureM;
	this->graphics = graphics;

	instance = this;
}

BoomBombPool::BoomBombPool(TextureManager * textureM, Graphics * graphics, int startNumberElement)
{
	this->textureManager = textureM; 
	this->graphics = graphics;

	for (int i = 0; i < startNumberElement; i++)
	{
		this->list.push_back(new BoomBomb(textureM, graphics));
	}

	instance = this;
}

BoomBombPool::~BoomBombPool()
{
}

BoomBomb * BoomBombPool::getBoom()
{
	if (this->list.empty())
	{
		return new BoomBomb(textureManager, graphics);
	}
	else
	{
		BoomBomb* bom = this->list.front();

		if (bom->isInStatus(eStatus::ENDING))
		{
			this->list.erase(this->list.begin());

			this->listUsing.push_back(bom);
			return bom;
		}	

		BoomBomb* boom = new BoomBomb(textureManager, graphics);
		this->listUsing.push_back(boom);
		return boom;
	}
}

void BoomBombPool::returnPool(BoomBomb * bom)
{
	bom->returnPool();
	this->list.push_back(bom);
	
	for (unsigned i = 0; i < this->listUsing.size(); i++)
	{
		if (this->listUsing[i] == bom)
			this->listUsing.erase(this->listUsing.begin() + i);
	}
}

std::vector<BoomBomb*> BoomBombPool::getListUsing()
{
	return this->listUsing;
}

void BoomBombPool::release()
{
	while (this->list.size())
	{
		BoomBomb *obj = this->list.front();
		this->list.erase(this->list.begin());
		delete obj;
	}

	delete instance;
}
