#include "Bullet.h"
#include "BulletPool.h"
#include "Collision.h"
#include "GateBlue.h"

#define WIDTH_BULLET_HALF 3
#define HEIGHT_BULLET_HALF 3

Bullet::Bullet(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::BULLET)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bullet");
	}

	// Set Data for sprite
	this->sprite->setData(IndexManager::getInstance()->samusYellowBulletNormal);
	this->setOrigin(VECTOR2(0.5f, 0.5f));

	this->setPosition(VECTOR2ZERO);

	this->distance = 0;

	this->listCollide = new list<CollisionReturn>();
}

Bullet::Bullet()
{

}


Bullet::~Bullet()
{
	delete this->sprite;
	this->listCollide->clear();
	delete this->listCollide;
}


void Bullet::update(float dt)
{
	if (this->distance < DISTANCE_SHOOT)
	{
		this->distance += VELOCITY*dt;
		this->setPosition(this->getPosition().x + this->getVelocity().x*dt, this->getPosition().y + this->getVelocity().y*dt);
		setBoundCollision();
	}
	else
	{
		this->setStatus(eStatus::ENDING);
		BulletPool::getInstance()->returnPool(this);
	}
}

void Bullet::draw()
{
	this->sprite->draw();
}

void Bullet::onCollision()
{
	for (auto i = this->listCollide->begin(); i != this->listCollide->end(); i++)
	{
		switch (i->object->getId())
		{
		case eID::WALL:
			BulletPool::getInstance()->returnPool(this);
			break;

		case eID::GATEBLUE:
		{
			BulletPool::getInstance()->returnPool(this);
			GateBlue* gate = static_cast<GateBlue*>(i->object);
			gate->setHit(true);
			break;
		}

		default:
			break;
		}
	}

	this->listCollide->clear();
}

void Bullet::setBoundCollision()
{
	MetroidRect rect;

	rect.left = getPosition().x - WIDTH_BULLET_HALF;
	rect.right = getPosition().x + WIDTH_BULLET_HALF;
	rect.top = getPosition().y - HEIGHT_BULLET_HALF;
	rect.bottom = getPosition().y + HEIGHT_BULLET_HALF;

	boundCollision = rect;
}

void Bullet::init(VECTOR2 stPosition)
{
	this->setPosition(stPosition);

	this->distance = 0;
	this->setStatus(eStatus::RUNNING);
}

void Bullet::returnPool()
{
	this->setPosition(VECTOR2ZERO);
}



list<CollisionReturn>* Bullet::getListCollide()
{
	return this->listCollide;
}

