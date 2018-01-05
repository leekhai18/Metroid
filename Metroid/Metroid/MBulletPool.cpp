#include "MBulletPool.h"
#include "Sound.h"

#define WIDTH_BULLET 50
#define TIME_DELAY_CREATE 0.1f
#define RATE_BEZIER 1.5f
MBulletPool::MBulletPool(TextureManager * textureM, Graphics * graphics, Samus * samus,int numberBullet)
{
	bulletPool = new map<int, MotherBrainBullet*>();
	for (int i = 0; i < numberBullet; i++)
	{
		MotherBrainBullet* bullet = new MotherBrainBullet(textureM, graphics);
		
		bullet->setMaxTime((1.0f*(i+1)) / numberBullet);
		
		bulletPool->insert(pair<int, MotherBrainBullet*>(i, bullet));
	}

	index = 0;

	finish = false;

	isCreateBullets = false;

	this->numberBullet = numberBullet;
	this->samus = samus;
}
void MBulletPool::setOwnPosition(VECTOR2 position)
{
	this->ownPosition = position;
	middlePosition = samus->getPosition();

	targetPosition = VECTOR2(middlePosition.x + WIDTH_BULLET, ownPosition.y);
	
	for (int i = 0; i < numberBullet; i++)
	{

		bulletPool->find(i)->second->setP1(ownPosition);
		bulletPool->find(i)->second->setP2(middlePosition);
		bulletPool->find(i)->second->setP3(targetPosition);
	}
}
void MBulletPool::reInit()
{
	middlePosition = VECTOR2(samus->getPosition().x,3552);

	targetPosition = VECTOR2(middlePosition.x + WIDTH_BULLET, ownPosition.y);

	finish = false;

	isCreateBullets = false;

	timeDelay = 0;

	time = 0;

	count = 0;

	for (auto bullet = bulletPool->begin(); bullet != bulletPool->end(); bullet++)
	{
		(*bullet).second->setStop(false);
		(*bullet).second->setCreate(false);
		(*bullet).second->setP1(ownPosition);
		(*bullet).second->setP2(middlePosition);
		(*bullet).second->setP3(targetPosition);
	}
}
void MBulletPool::handleVelocity(float dt)
{

	if(finish)
	{
		reInit();
		return;
	}
	if (!isCreateBullets)
	{
		timeDelay += dt;
		count = timeDelay / TIME_DELAY_CREATE;
		if (bulletPool->find(9)->second->getCreate())
		{
			int test = 0;
		}
		if(count >= (numberBullet - 1)&&bulletPool->find(numberBullet-1)->second->getCreate())
		{
			isCreateBullets = true;

			distance = targetPosition - middlePosition;

			beginPosition = targetPosition;
		}
		if(count >= (numberBullet - 1))
		{
			count = (numberBullet - 1);
		}
		for (int i = 0; i < count + 1; i++)
		{
			if(!bulletPool->find(i)->second->getCreate())
			{
				bulletPool->find(i)->second->handleVelocity(dt);
			}
		}
		
	}
	else
	{
		time += dt*RATE_BEZIER;

		if(time >=1)
		{
			time = 1;
		}
		targetPosition = beginPosition + time*(middlePosition - beginPosition);

		for (auto bullet = bulletPool->begin(); bullet != bulletPool->end(); bullet++)
		{
			(*bullet).second->setP3(targetPosition);
			(*bullet).second->handleVelocity(dt);
		}

		if(time == 1)
		{
			finish = true;
			for (auto bullet = bulletPool->begin(); bullet != bulletPool->end(); bullet++)
			{
				(*bullet).second->setStop(true);
			}
		}
	}
}

void MBulletPool::onCollision(float dt)
{
	for (auto bullet = bulletPool->begin(); bullet != bulletPool->end(); bullet++)
	{
		(*bullet).second->onCollision(samus,dt);
	}
}

void MBulletPool::update(float dt)
{
	for (auto bullet = bulletPool->begin(); bullet != bulletPool->end(); bullet++)
	{
		(*bullet).second->update(dt);
	}
}

void MBulletPool::draw()
{
	for (auto bullet = bulletPool->begin(); bullet != bulletPool->end(); bullet++)
	{
		(*bullet).second->draw();
	}
}



MBulletPool::MBulletPool()
{
}


MBulletPool::~MBulletPool()
{
	for (auto i = bulletPool->begin(); i !=bulletPool->end(); i++)
	{
		delete (*i).second;
	}
	delete bulletPool;
}
