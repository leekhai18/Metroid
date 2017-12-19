#include "RocketPool.h"

RocketPool* RocketPool::instance = nullptr;

RocketPool * RocketPool::getInstance()
{
	return instance;
}

RocketPool::RocketPool(TextureManager* textureM, Graphics* graphics)
{
	this->textureManager = textureM;
	this->graphics = graphics;

	instance = this;
}

RocketPool::RocketPool(TextureManager * textureM, Graphics * graphics, int startNumberElement)
{
	this->textureManager = textureM; 
	this->graphics = graphics;

	for (int i = 0; i < startNumberElement; i++)
	{
		this->list.push_back(new Rocket(textureM, graphics));
	}

	instance = this;
}

RocketPool::~RocketPool()
{
}

Rocket * RocketPool::getRocket()
{
	if (this->list.empty())
	{
		Rocket* roc = new Rocket(textureManager, graphics);
		this->listUsing.push_back(roc);
		return roc;
	}
	else
	{
		Rocket* rocket = this->list.front();

		if (rocket->isInStatus(eStatus::ENDING))
		{
			this->list.erase(this->list.begin());

			this->listUsing.push_back(rocket);
			return rocket;
		}	

		Rocket* roc = new Rocket(textureManager, graphics);
		this->listUsing.push_back(roc);
		return roc;
	}
}

void RocketPool::returnPool(Rocket * bullet)
{
	bullet->returnPool();
	this->list.push_back(bullet);
	
	for (unsigned i = 0; i < this->listUsing.size(); i++)
	{
		if (this->listUsing[i] == bullet)
			this->listUsing.erase(this->listUsing.begin() + i);
	}
}

std::vector<Rocket*> RocketPool::getListUsing()
{
	return this->listUsing;
}

void RocketPool::release()
{
	while (this->list.size())
	{
		Rocket *obj = this->list.front();
		this->list.erase(this->list.begin());
		delete obj;
	}

	delete instance;
}

