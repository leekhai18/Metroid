#include "IceBeam.h"
#include "Metroid.h"

IceBeam::IceBeam()
{
}

IceBeam::IceBeam(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ICEBEAM)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite IceBeam");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->iceBeam, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();

	this->isActivity = true;
}


IceBeam::~IceBeam()
{
	delete this->anim;
	delete this->sprite;
}

void IceBeam::update(float dt)
{
	if (this->isActivity)
		this->anim->update(dt);
}

void IceBeam::draw()
{
	if (this->isActivity)
		this->sprite->draw();
}

void IceBeam::setActivity(bool flag)
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

bool IceBeam::getActivity()
{
	return this->isActivity;
}
