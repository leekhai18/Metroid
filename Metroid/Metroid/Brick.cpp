#include "Brick.h"

Brick::Brick()
{
}

Brick::Brick(TextureManager * textureM, Graphics * graphics, BrickStyle style) : BaseObject(eID::BRICK)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Brick");
	}

	switch (style)
	{
	case BrickSecretBlue:
		this->sprite->setData(IndexManager::getInstance()->brickSecretBlue);
		break;
	case BrickSerectGreen:
		this->sprite->setData(IndexManager::getInstance()->brickSecretGreen);
		break;
	case BrickGreen:
		this->sprite->setData(IndexManager::getInstance()->brickGreen);
		break;
	case BrickBlue:
		this->sprite->setData(IndexManager::getInstance()->brickBlue);
		break;
	case BrickRed:
		this->sprite->setData(IndexManager::getInstance()->brickRed);
		break;
	default:
		break;
	}

	this->isVisible = true;
}


Brick::~Brick()
{
	delete this->sprite;
}

void Brick::update(float dt)
{
}

void Brick::draw()
{
	if (this->isVisible == true)
		this->sprite->draw();
}

void Brick::setVisible(bool visible)
{
	this->isVisible = visible;
}
