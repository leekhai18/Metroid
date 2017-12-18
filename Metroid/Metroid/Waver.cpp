#include "Collision.h"
#include "Waver.h"
#include "Camera.h"
#define TIME_FRAME_DELAY 0.5f
#define MAX_WAVER_VELOCITY_Y 100
#define WAVER_VELOCITY_X 100
#define WAVER_ACCELERATE_Y 100
#define TIME_TO_CHANGE_STATE 0.5f
Waver::Waver()
{

}

Waver::Waver(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::WAVER)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Waver");
	}

	switch (color)
	{
	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->waverBrown, NUM_FRAMES_WAVER, TIME_FRAME_DELAY);
		health = 4;
		break;

	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->waverRed, NUM_FRAMES_WAVER, TIME_FRAME_DELAY);
		health = 8;
		break;

	default:
		break;
	}

	this->anim->start();



	this->sprite->setOrigin(VECTOR2(0.5, 0.5));

	this->velocity_frame = MAX_WAVER_VELOCITY_Y;

	direction = eDirection::left;
	directionY = WaverDirectionY::up;

	this->velocity = VECTOR2(WAVER_VELOCITY_X*direction, MAX_WAVER_VELOCITY_Y*directionY);
	this->listWallCanCollide = new list<BaseObject*>();
	this->listCollide = new list<CollisionReturn>();
}


Waver::~Waver()
{
	delete anim;
}
list<BaseObject*>* Waver::getListWallCanCollide()
{
	return this->listWallCanCollide;
}
list<CollisionReturn>* Waver::getListCollide()
{
	return this->listCollide;
}
void Waver::setStartPosition(VECTOR2 position)
{
	this->startPosition = position;
}
void Waver::onCollision(float dt)
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
			direction = eDirection::right;
			this->sprite->setFlipX(true);
			break;
		case CollideDirection::RIGHT:
			direction = eDirection::left;
			this->sprite->setFlipX(true);
			break;
		case CollideDirection::TOP:

			break;
		case CollideDirection::BOTTOM:

			break;
		}
	}

	this->listCollide->clear();
}
void Waver::update(float dt)
{
	this->anim->update(dt);

	

	if (directionY == WaverDirectionY::up)
	{
		this->velocity_frame = velocity.y - WAVER_ACCELERATE_Y*dt;
		if (velocity_frame <= 0)
		{
			velocity.y = -MAX_WAVER_VELOCITY_Y;
			this->velocity_frame = velocity.y + WAVER_ACCELERATE_Y*dt;
			directionY = WaverDirectionY::down;
		}
	}
	else
	{
		this->velocity_frame = velocity.y + WAVER_ACCELERATE_Y*dt;
		if (velocity_frame >= 0)
		{
			velocity.y = MAX_WAVER_VELOCITY_Y;
			this->velocity_frame = velocity.y - WAVER_ACCELERATE_Y*dt;
			directionY = WaverDirectionY::up;
		}
	}



	this->velocity.y = (velocity_frame + velocity.y)*0.5f;

	// nhanh dần -> chậm dần, 1 chu kỳ thì 1 turn anim,
	this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt));

	this->velocity.y = velocity_frame;

	if (this->getPosition().x  < Camera::getInstance()->getBound().left || this->getPosition().x > Camera::getInstance()->getBound().right)
	{
	
	}
}



void Waver::draw()
{
	this->sprite->draw();
}
