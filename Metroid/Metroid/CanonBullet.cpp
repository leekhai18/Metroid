#include "CanonBullet.h"



CanonBullet::CanonBullet()
{
}

CanonBullet::CanonBullet(Graphics * graphics, TextureManager * textureM, CanonType type)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
	}

	switch (type)
	{
	case CanonType::CANON_LEFT:
		//this->sprite->setData(IndexManager::getInstance()->ca)
		break;
	case CanonType::CANON_RIGHT:
		break;
	case CanonType::CANON_TOP:
		break;
	default:
		break;
	}
}

void CanonBullet::init()
{
}

void CanonBullet::end()
{
}

void CanonBullet::setBoundCollision()
{
}

void CanonBullet::handleVelocity(float dt)
{
}

void CanonBullet::onCollision(float dt)
{
}

void CanonBullet::update(float dt)
{
}

void CanonBullet::draw()
{
}


CanonBullet::~CanonBullet()
{
}
