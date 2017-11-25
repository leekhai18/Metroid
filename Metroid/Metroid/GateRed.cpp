#include "GateRed.h"

GateRed::GateRed()
{
}

GateRed::GateRed(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::GATERED)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite GateRed");
	}

	this->sprite->setData(IndexManager::getInstance()->gateRedR);
}


GateRed::~GateRed()
{
	delete this->sprite;
}

void GateRed::update(float dt)
{
}

void GateRed::draw()
{
	this->sprite->draw();
}
