#include "Zommer.h"


Zommer::Zommer(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ZOMMER)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
	}
	this->setPosition(VECTOR2(0, 200));

	this->sprite->setOrigin(VECTOR2(0.5f, 0.5f));

	animation = new Animation(this->sprite, IndexManager::getInstance()->zoomerRed, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
	animation->start();

	
}

Zommer::Zommer()
{
}


Zommer::~Zommer()
{
	
}

void Zommer::update(float dt)
{
	this->animation->update(dt);
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

void Zommer::release()
{
	delete this->sprite;
	delete this->animation;

	this->sprite = nullptr;
	this->animation = nullptr;
}
