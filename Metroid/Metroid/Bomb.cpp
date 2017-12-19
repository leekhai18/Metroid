#include "Bomb.h"
#include "Metroid.h"

Bomb::Bomb()
{
}

Bomb::Bomb(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::BOMB)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Bomb");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->bomb, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();

	this->isActivity = true;
}


Bomb::~Bomb()
{
	delete this->anim;
	delete this->sprite;
}

void Bomb::update(float dt)
{
	if (isActivity)
	{
		this->anim->update(dt);
	}
}

void Bomb::draw()
{
	if (isActivity)
	{
		this->sprite->draw();
	}
}

void Bomb::setActivity(bool flag)
{
	this->isActivity = flag;

	if (this->isActivity == true)
	{
		MetroidRect bound;

		bound.left = this->getPosition().x;
		bound.top = this->getPosition().y;
		bound.right = bound.left + this->getSprite()->getWidth();
		bound.bottom = bound.top - this->getSprite()->getHeight();

		this->setBoundCollision(bound);
		this->setActiveBound(bound);
	}
	else
	{
		MetroidRect bound(0, 0, 0, 0);

		this->setBoundCollision(bound);
		this->setActiveBound(bound);

		Metroid::getInstance()->setJustCollectItem(true);
	}
}

bool Bomb::getActivity()
{
	return this->isActivity;
}
