#include "Zommer.h"
#define TIME_FRAME_DELAY 0.15f

Zommer::Zommer(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ZOMMER)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
	}
	this->setPosition(VECTOR2(0, 200));

	this->sprite->setOrigin(VECTOR2(0.5f, 0.5f));

	anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerRed, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
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
	this->setPosition(VECTOR2(this->getPosition().x + 3 * change, this->getPosition().y));
	if (this->getPosition().x > GAME_WIDTH) {
		change = -1;
	}
	if (this->getPosition().x == 0) {
		change = 1;
	}
}

void Zommer::draw()
{
		this->sprite->draw();
}
