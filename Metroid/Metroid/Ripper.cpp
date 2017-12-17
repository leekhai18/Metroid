#include "Ripper.h"

Ripper::Ripper()
{
}

Ripper::Ripper(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::RIPPER)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Ripper");
	}

	switch (color)
	{
	case Yellow:
		this->sprite->setData(IndexManager::getInstance()->ripperYellow);
		break;

	case Brown:
		this->sprite->setData(IndexManager::getInstance()->ripperBrown);
		break;

	case Red:
		this->sprite->setData(IndexManager::getInstance()->ripperRed);
		break;

	default:
		break;
	}
	this->listWallCanCollide = new list<BaseObject*>();
	this->listCollide = new list<CollisionReturn>();
}


Ripper::~Ripper()
{
	delete this->sprite;
}

list<BaseObject*>* Ripper::getListWallCanCollide()
{
	return this->listWallCanCollide;
}

list<CollisionReturn>* Ripper::getListCollide()
{
	return this->listCollide;
}

void Ripper::update(float dt)
{
}

void Ripper::draw()
{
	this->sprite->draw();
}
