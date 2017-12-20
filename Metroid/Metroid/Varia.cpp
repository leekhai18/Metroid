#include "Varia.h"
#include "Metroid.h"

Varia::Varia()
{
}

Varia::Varia(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::VARIA)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Varia");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->varia, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();

	this->isActivity = true;
}


Varia::~Varia()
{
	delete this->anim;
	delete this->sprite;
}

void Varia::update(float dt)
{
	if (this->isActivity)
		this->anim->update(dt);
}

void Varia::draw()
{
	if (this->isActivity)
		this->sprite->draw();
}

void Varia::setActivity(bool flag)
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

bool Varia::getActivity()
{
	return this->isActivity;
}
