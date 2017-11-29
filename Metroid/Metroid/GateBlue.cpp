#include "GateBlue.h"
#include "Camera.h"

GateBlue::GateBlue()
{
}

GateBlue::GateBlue(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::GATEBLUE)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite GateBlue");
	}

	this->sprite->setData(IndexManager::getInstance()->gateBlueR);
}


GateBlue::~GateBlue()
{
	delete this->sprite;
}

void GateBlue::update(float dt)
{

}

void GateBlue::draw()
{
	this->sprite->draw();
}
