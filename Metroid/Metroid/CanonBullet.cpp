#include "CanonBullet.h"



CanonBullet::CanonBullet()
{
}

CanonBullet::CanonBullet(Graphics * graphics, TextureManager * textureM)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
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
