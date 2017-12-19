#include "MissileRocket.h"
#include "Metroid.h"

MissileRocket::MissileRocket()
{
}

MissileRocket::MissileRocket(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::MISSILEROCKET)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite MissileRocket");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->missileRocket, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();
}


MissileRocket::~MissileRocket()
{
	delete this->anim;
	delete this->sprite;
}

void MissileRocket::update(float dt)
{
	if (this->isActivity)
		this->anim->update(dt);
}

void MissileRocket::draw()
{
	if (this->isActivity)
		this->sprite->draw();
}

void MissileRocket::setActivity(bool flag)
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

bool MissileRocket::getActivity()
{
	return this->isActivity;
}
