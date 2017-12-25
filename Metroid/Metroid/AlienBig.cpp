#include "AlienBig.h"

AlienBig::AlienBig()
{
}

AlienBig::AlienBig(TextureManager * textureM, Graphics * graphics) : BaseObject(eID::ALIENBIG)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite AlienBig");
	}

	this->sprite->setData(IndexManager::getInstance()->alienBigR);
	listBrick = new list<Brick*>();
}

void AlienBig::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->getSprite()->getWidth() *0.5f;
	rect.right = position.x + this->getSprite()->getWidth() *0.5f;
	rect.top = position.y + this->getSprite()->getHeight()*0.5f;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f;

	this->boundCollision = rect;
}
AlienBig::~AlienBig()
{
	delete this->sprite;
	delete listBrick;
}

list<Brick*>* AlienBig::getListBrick()
{
	return this->listBrick;
}

void AlienBig::setBeHit()
{
	beHit = true;
	
}

bool AlienBig::getBeHit()
{
	return this->beHit;
}

void AlienBig::update(float dt)
{
}

void AlienBig::draw()
{
	this->sprite->draw();
}
