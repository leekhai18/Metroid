#include "AlienSmall.h"

AlienSmall::AlienSmall()
{
}

AlienSmall::AlienSmall(TextureManager * textureM, Graphics * graphics,AlienBig* big) : BaseObject(eID::ALIENSMALL)
{
	this->sprite = new Sprite();
	this->big = big;
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite AlienSmall");
	}

	this->sprite->setData(IndexManager::getInstance()->alienSmallR);
}


AlienSmall::~AlienSmall()
{
	delete this->sprite;
}

void AlienSmall::setBeHit()
{
	if (!big->getBeHit()||beHit)
	{
		return;
	}
	for (auto i = big->getListBrick()->begin(); i != big->getListBrick()->end(); ++i)
	{
		(*i)->setVisible(true);
	}
	this->beHit = true;
}

void AlienSmall::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->getSprite()->getWidth() *0.5f ;
	rect.right = position.x + this->getSprite()->getWidth() *0.5f ;
	rect.top = position.y + this->getSprite()->getHeight()*0.5f ;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f ;

	this->boundCollision = rect;
}

void AlienSmall::update(float dt)
{
}

void AlienSmall::draw()
{
	this->sprite->draw();
}
