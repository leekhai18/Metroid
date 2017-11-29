#include "Zommer.h"
#define TIME_FRAME_DELAY 0.15f

Zommer::Zommer(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::ZOMMER)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
	}

	switch (color)
	{
	case Yellow:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerYellow, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
		health = 2;
		break;

	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerBrown, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
		health = 2;
		break;

	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerRed, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
		health = 4;
		break;

	default:
		break;
	}

	anim->start();
}

Zommer::Zommer()
{
}


Zommer::~Zommer()
{
	delete this->anim;
}

void Zommer::update(float dt)
{
	this->anim->update(dt);
}

void Zommer::draw()
{
		this->sprite->draw();
}
