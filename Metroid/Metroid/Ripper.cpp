#include "Ripper.h"
#define TIME_FRAME_DELAY 0.15f
#define RIPPER_VELOCITY_X 40
#define RIPPER_OFFSET_COLLISION 0.1f

Ripper::Ripper()
{
}

Ripper::Ripper(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::RIPPER), IFreezable(IndexManager::getInstance()->ripperBlue)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Ripper");
	}

	switch (color)
	{
	case Yellow:
		this->sprite->setData(IndexManager::getInstance()->ripperYellow);
		break;

	case Brown:
		this->sprite->setData(IndexManager::getInstance()->ripperBrown);
		break;

	case Red:
		this->sprite->setData(IndexManager::getInstance()->ripperRed);
		break;

	default:
		break;
	}
	this->setOrigin(VECTOR2(0.5, 0.5));
	this->setVelocityX(-RIPPER_VELOCITY_X);
	this->setVelocityY(0);
	this->listWallCanCollide = new list<BaseObject*>();
	this->listCollide = new list<CollisionReturn>();

}


Ripper::~Ripper()
{
	delete this->sprite;
	delete listWallCanCollide;
	delete listCollide;
}

void Ripper::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - this->getSprite()->getWidth() *0.5f ;
	rect.right = position.x + this->getSprite()->getWidth() *0.5f;
	rect.top = position.y + this->getSprite()->getHeight() *0.5f;
	rect.bottom = position.y - this->getSprite()->getHeight() *0.5f;


	this->boundCollision = rect;
}

void Ripper::onCollision(float dt)
{
	for (auto i = this->getListWallCanCollide()->begin(); i != this->getListWallCanCollide()->end(); i++)
	{
		Collision::getInstance()->checkCollision(this, *i, dt);
	}

	for (auto x = this->listCollide->begin(); x != this->listCollide->end(); x++)
	{
		switch (x->direction)
		{
		case CollideDirection::LEFT:
			this->sprite->setFlipX(false);
			this->setVelocityX(-RIPPER_VELOCITY_X);
			break;
		case CollideDirection::RIGHT:
			this->sprite->setFlipX(true);
			this->setVelocityX(RIPPER_VELOCITY_X);
			break;
		case CollideDirection::TOP:
			
			break;
		case CollideDirection::BOTTOM:
			
			break;
		}
	}

	this->listCollide->clear();
}

void Ripper::update(float dt)
{
	if (this->isCold)
	{
		this->sprite->setData(this->frameID[IndexManager::getInstance()->ripperBlue[0]]);
		return;
	}

	this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt));
	setBoundCollision();
}

void Ripper::draw()
{
	this->sprite->draw();
}
list<BaseObject*>* Ripper::getListWallCanCollide()
{
	return this->listWallCanCollide;
}

list<CollisionReturn>* Ripper::getListCollide()
{
	return this->listCollide;
}

