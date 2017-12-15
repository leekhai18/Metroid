#include "Zommer.h"
#define TIME_FRAME_DELAY 0.15f

Zommer::Zommer(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::ZOMMER)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
	}

	switch (color)
	{
	case Yellow:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerYellow, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
		health = 2;
		break;

	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerBrown, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
		health = 2;
		break;

	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerRed, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
		health = 4;
		break;

	default:
		break;
	}
	
	this->listWallCanCollide = new list<BaseObject*>();
	this->listCollide = new list<CollisionReturn>();
	anim->start();

	this->setVelocityX(40);

	this->setVelocityY(0);
}

Zommer::Zommer()
{
}


Zommer::~Zommer()
{
	delete this->listWallCanCollide;
	delete this->listCollide;
	delete this->anim;
}

void Zommer::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x;
	rect.right = position.x + this->getSprite()->getWidth();
	rect.top = position.y;
	rect.bottom = position.y - this->getSprite()->getHeight();
	this->boundCollision = rect;
}

void Zommer::onCollision(float dt)
{
	for (auto i = this->getListWallCanCollide()->begin(); i != this->getListWallCanCollide()->end(); i++)
	{
		Collision::getInstance()->checkCollision(this, *i, dt);
	}
	for (auto x = this->getListCollide()->begin(); x != this->getListCollide()->end(); x++)
	{
		bound = x->object->getBoundCollision();
		switch (x->direction)
		{
		
		case CollideDirection::LEFT:
			this->setVelocityX(70);
			this->setVelocityY(70);
			this->x = -(this->velocity.x*dt);
			this->y = 0;
			isCollide = true;
			flag = true;
			break;
		case CollideDirection::RIGHT:
			this->setVelocityX(-70);
			this->setVelocityY(-60);
			this->x = -(this->velocity.x*dt);
			this->y = 0;
			flag = true;
			break;
		case CollideDirection::TOP:
			this->setVelocityX(60);
			this->setVelocityY(-60);
			this->x = 0;
			this->y = -(this->velocity.y*dt);
			flag = true;
			break;
		case CollideDirection::BOTTOM:
			this->setVelocityX(-60);
			this->setVelocityY(60);
			this->x = 0;
			this->y = -(this->velocity.y*dt);
			flag = true;
			break;
		}
	}

	this->listCollide->clear();
}
void Zommer::update(float dt)
{
	//if not collide top-left
	if (flag == false && this->velocity.x > 0 && this->velocity.y > 0)
	{
		this->setVelocityX(60);
		this->setVelocityY(-60);
		this->x = 0;
		this->y = -2*(this->velocity.y*dt);
		flag = true;
	}
	//if not collide top-right
	if (flag == false && this->velocity.x > 0 && this->velocity.y < 0)
	{
		this->setVelocityX(-60);
		this->setVelocityY(-60);
		this->x = -2*(this->velocity.x*dt);
		this->y = 0;
		flag = true;
	}
	//if not collide bottom-right
	if (flag == false && this->velocity.x < 0 && this->velocity.y < 0)
	{
		this->setVelocityX(-60);
		this->setVelocityY(60);
		this->x = 0;
		this->y = -2*(this->velocity.y*dt);
		flag = true;

	}
	//if not collide bottom-left
	if (flag == false && this->velocity.x < 0 && this->velocity.y > 0)
	{
		this->setVelocityX(60);
		this->setVelocityY(60);
		this->x = -2*(this->velocity.x*dt);
		this->y = 0;
		flag = true;
	}
	
	this->anim->update(dt);
	this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt + x, this->getPosition().y+ this->velocity.y*dt + y));
	setBoundCollision();
	if (isCollide == true)
	{
		flag = false;
	}
}

void Zommer::draw()
{
		this->sprite->draw();
}

list<BaseObject*>* Zommer::getListWallCanCollide()
{
	return this->listWallCanCollide;
}
list<CollisionReturn>* Zommer::getListCollide()
{
	return this->listCollide;
}

