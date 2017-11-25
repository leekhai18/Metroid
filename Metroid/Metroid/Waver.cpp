﻿#include "Waver.h"



Waver::Waver()
{
}

Waver::Waver(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::WAVER)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Waver");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->waverBrown, NUM_FRAMES_WAVER, 0.5f);
	this->anim->start();

	this->setPosition(VECTOR2(640, 3200));
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
