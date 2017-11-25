#include "Waver.h"
#define TIME_FRAME_DELAY 0.5f



Waver::Waver()
{
}

Waver::Waver(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::WAVER)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Waver");
	}

	switch (color)
	{
	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->waverBrown, NUM_FRAMES_WAVER, TIME_FRAME_DELAY);
		health = 4;
		break;

	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->waverRed, NUM_FRAMES_WAVER, TIME_FRAME_DELAY);
		health = 8;
		break;

	default:
		break;
	}

	this->anim->start();
}


Waver::~Waver()
{
	delete anim;
}

void Waver::update(float dt)
{
	this->anim->update(dt);

	// nhanh dần -> chậm dần, 1 chu kỳ thì 1 turn anim, 
}

void Waver::draw()
{
	this->sprite->draw();
}
