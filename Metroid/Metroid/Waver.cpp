#include "Collision.h"
#include "Waver.h"
#include "Camera.h"
#define TIME_FRAME_DELAY 0.5f
#define MAX_WAVER_VELOCITY_Y 100
#define WAVER_VELOCITY_X 50
#define WAVER_ACCELERATE_Y 100
#define TIME_TO_CHANGE_STATE 0.5f
#define TIME_DELAY_BE_HIT 0.2f
#define TIME_RETURN_NOMAL 1.0f
#define WAVER_DIMENSION 16
#define WAVER_OFFSET 0.5f
Waver::Waver()
{

}

Waver::Waver(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::WAVER), IFreezable(IndexManager::getInstance()->waverBlue)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Waver");
	}
	initialize(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY);

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

	isActivity = true;


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
void Waver::reInit()
{
	this->setPosition(startPosition);
	isActivity = true;
	canDraw = true;
	health = 2;
	this->explosion->reInit();
	this->anim->setPause(false);
}
void Waver::setBeHit(bool hit)
{
	this->beHit = hit;
}
void Waver::decreaseHealth(float dame)
{
	this->health = this->health - dame;
}
void Waver::handleVelocity(float dt)
{
	if (isActivity)
	{
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
		this->velocity.x = WAVER_VELOCITY_X*direction;
	}

}
void Waver::setStartBound(MetroidRect rect)
{
	this->startBound = rect;
}

void Waver::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - WAVER_DIMENSION *0.5f + WAVER_OFFSET;
	rect.right = position.x + WAVER_DIMENSION *0.5f - WAVER_OFFSET;
	rect.top = position.y + WAVER_DIMENSION *0.5f - WAVER_OFFSET;
	rect.bottom = position.y - WAVER_DIMENSION *0.5f + WAVER_OFFSET;

	this->boundCollision = rect;
}
void Waver::onCollision(float dt)
{
	if (isActivity)
	{
		for (auto i = this->listWallCanCollide->begin(); i != this->listWallCanCollide->end(); i++)
		{
			Collision::getInstance()->checkCollision(this, *i, dt);
		}
		for (auto x = this->listCollide->begin(); x != this->listCollide->end(); x++)
		{

			switch (x->direction)
			{
			case CollideDirection::LEFT:
				direction = eDirection::left;
				this->sprite->setFlipX(true);
				break;
			case CollideDirection::RIGHT:
				direction = eDirection::right;
				this->sprite->setFlipX(false);
				break;
			case CollideDirection::TOP:
				velocity.y = 0;
				directionY = WaverDirectionY::down;
				break;
			case CollideDirection::BOTTOM:
				velocity.y = 0;
				directionY = WaverDirectionY::up;
				break;
			}
		}

		this->listCollide->clear();

		switch (direction)
		{
		case eDirection::left:
			if (boundCollision.left + velocity.x*dt <= Camera::getInstance()->getBound().left)
			{
				this->velocity.x = 0;
				direction = eDirection::right;
			}
			break;
		case eDirection::right:
			if (boundCollision.right + velocity.x*dt >= Camera::getInstance()->getBound().right)
			{
				direction = eDirection::left;
				this->velocity.x = 0;
			}
			break;
		default:
			break;
		}
	}
}
void Waver::update(float dt)
{
	if (isActivity)
	{
		if (this->isCold)
		{
			timeReturnNormal += dt;
			if (timeReturnNormal >= TIME_RETURN_NOMAL)
			{
				this->anim->setPause(false);
				this->isCold = false;
			}
			else
			{
				this->sprite->setData(this->frameID[anim->getCurrentFrame()]);
				this->anim->setPause(true);
				return;
			}

		}
		this->anim->update(dt);

		if (beHit)
		{
			timerHit += dt;
			if (timerHit < TIME_DELAY_BE_HIT)
			{
				this->anim->setPause(true);
				this->setVelocity(VECTOR2(0, 0));
			}
			else
			{
				timerHit = 0;
				beHit = false;
				this->anim->setPause(false);

				if (this->health <= 0)
				{

					IExplosible::start();
					this->setVelocity(VECTOR2(0, 0));
					this->isActivity = false;
				}
			}
		}

	
		// nhanh dần -> chậm dần, 1 chu kỳ thì 1 turn anim,
		this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt));
		setBoundCollision();

		this->velocity.y = velocity_frame;
	
	}
	IExplosible::update(dt);


	if (!Collision::getInstance()->isCollide(Camera::getInstance()->getBound(),this->startBound)
		&& !Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->boundCollision))
	{
		reInit();
	}
}



void Waver::draw()
{
	if(canDraw)
	{
		this->sprite->draw();
	}
	
}
