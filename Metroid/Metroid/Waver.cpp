#include "Waver.h"



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

	this->sprite->setOrigin(VECTOR2(0.5f, 0.5f));

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->waverBrown, NUM_FRAMES_WAVER, 0.5f);
	this->anim->start();

	this->setPosition(VECTOR2(100, 100));
}


Waver::~Waver()
{
	delete sprite;
	delete anim;
}

void Waver::update(float dt)
{
	this->anim->update(dt);

	// nhanh dần -> chậm dần, 1 chu kỳ thì 1 turn anim, 
}

void Waver::draw()
{
	if (this->camera)
		this->sprite->setTransformCamera(VECTOR2(GAME_WIDTH*0.5f - camera->getPosition().x, GAME_HEIGHT*0.8f - camera->getPosition().y));

	this->sprite->draw();
}

void Waver::setCamera(Camera * cam)
{
	this->camera = cam;
}
