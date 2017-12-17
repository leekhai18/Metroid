#include "EnergyTank.h"
#include "Metroid.h"

EnergyTank::EnergyTank()
{
}

EnergyTank::EnergyTank(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ENERGYTANK)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite EnergyTank");
	}

	this->anim = new Animation(this->sprite, IndexManager::getInstance()->energyTank, NUM_FRAMES_ITEM, TIME_FRAME_ITEM);
	this->anim->start();

	this->isActivity = true;
}


EnergyTank::~EnergyTank()
{
	delete this->anim;
	delete this->sprite;
}

void EnergyTank::update(float dt)
{
	if (this->isActivity)
		this->anim->update(dt);
}

void EnergyTank::draw()
{
	if (this->isActivity)
		this->sprite->draw();
}

void EnergyTank::setActivity(bool flag)
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

bool EnergyTank::getActivity()
{
	return this->isActivity;
}
