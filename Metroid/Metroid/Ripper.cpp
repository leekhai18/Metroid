#include "Ripper.h"
#define TIME_FRAME_DELAY 0.15f
#define RIPPER_VELOCITY_X 40
#define RIPPER_OFFSET_COLLISION 0.1f
#define TIME_RETURN_NOMAL 5.0f
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
		resetFrame = IndexManager::getInstance()->ripperYellow;
		break;

	case Brown:
		this->sprite->setData(IndexManager::getInstance()->ripperBrown);
		resetFrame = IndexManager::getInstance()->ripperBrown;
		break;

	case Red:
		this->sprite->setData(IndexManager::getInstance()->ripperRed);
		resetFrame = IndexManager::getInstance()->ripperRed;
		break;

	default:
		break;
	}
	this->setOrigin(VECTOR2(0.5, 0.5));
	this->setVelocityX(-RIPPER_VELOCITY_X);
	this->setVelocityY(0);
	this->listWallCanCollide = new map<int,BaseObject*>();
	this->listCollide = new list<CollisionReturn>();

}


Ripper::~Ripper()
{
	delete this->sprite;
	delete listWallCanCollide;
	delete listCollide;
}

void Ripper::handleVelocity(float dt)
{
	if(!isCold)
	{
		this->velocity.x = RIPPER_VELOCITY_X*direction;
	}
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
	/*for (auto i = this->listCanCollide->begin(); i != this->listCanCollide->end(); i++)
	{
		BaseObject* x = (*i).second;
		Collision::getInstance()->checkCollision(this, x, dt);
	}*/

	for (auto x = this->listCollide->begin(); x != this->listCollide->end(); x++)
	{
		switch (x->direction)
		{
		case CollideDirection::LEFT:
			this->sprite->setFlipX(false);
			//this->setVelocityX(-RIPPER_VELOCITY_X);
			this->velocity.x = 0;
			direction = eDirection::left;
			break;
		case CollideDirection::RIGHT:
			this->sprite->setFlipX(true);
			this->velocity.x = 0;
			direction = eDirection::right;
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
		timeReturnNormal += dt;
		if (timeReturnNormal >= TIME_RETURN_NOMAL)
		{
			this->isCold = false;
			this->sprite->setData(resetFrame);
		}
		else
		{
			this->sprite->setData(this->frameID[0]);
			this->setVelocity(VECTOR2(0, 0));
			return;
		}

	}
	this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt));
	setBoundCollision();
}

void Ripper::draw()
{
	this->sprite->draw();
}
map<int,BaseObject*>* Ripper::getListWallCanCollide()
{
	return this->listWallCanCollide;
}

list<CollisionReturn>* Ripper::getListCollide()
{
	return this->listCollide;
}

