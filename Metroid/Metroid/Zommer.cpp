#include "Zommer.h"
#include "Camera.h"
#define TIME_FRAME_DELAY 0.15f
#define ZOMMER_VELOCITY_X 40
#define ZOMMER_VELOCITY_Y 40
#define ZOMMER_OFFSET_COLLISION 0.1f
#define TIME_DELAY_BE_HIT 0.2f
#define TIME_RETURN_NOMAL 5.0f
#include "Sound.h"
Zommer::Zommer(TextureManager * textureM, Graphics * graphics, Samus* samus,EnemyColors color) : BaseObject(eID::ZOMMER),IFreezable(IndexManager::getInstance()->zoomerBlue)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
	}
	this->samus = samus;
	this->initExplosion(this->sprite, IndexManager::getInstance()->samusYellowExplosion, NUM_FRAMES_EXPLOSION, EXPLOSION_TIME_FRAME_DELAY);

	this->initRocket(this->sprite, IndexManager::getInstance()->rocket, NUM_FRAMES_BONUS, TIME_FRAME_DELAY);
	this->initItem(this->sprite, IndexManager::getInstance()->bonusHealth, NUM_FRAMES_BONUS, TIME_FRAME_DELAY);

	this->color = color;
	switch (color)
	{
	case Yellow:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerYellow, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
		reset = IndexManager::getInstance()->zoomerYellow[0];
		health = 2;
		break;

	case Brown:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerBrown, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
		reset = IndexManager::getInstance()->zoomerBrown[0];
		health = 2;
		break;

	case Red:
		anim = new Animation(this->sprite, IndexManager::getInstance()->zoomerRed, NUM_FRAMES_ZOOMER, TIME_FRAME_DELAY);
		reset = IndexManager::getInstance()->zoomerRed[0];
		health = 4;
		break;

	default:
		break;
	}

	//this->topSide=()
	this->setOrigin(VECTOR2(0.5, 0.5));
	
	this->listWallCanCollide = new map<int,BaseObject*>();
	this->listCollide = new list<CollisionReturn>();
	anim->start();

	//this->setPosition(VECTOR2(679, 1367));

	zommer_direction = ZommerDirection::LEFT_DIRECTION;
	gravity = ZommerGravity::GRAVITY_BOTTOM;

	/*this->velocity.x = ZOMMER_VELOCITY_Y;

	this->velocity.y = -ZOMMER_VELOCITY_X;*/
	
	for (size_t i = 0; i < 4; i++)
	{
		gravity_bool[i] = false;
	}
	setBoundCollision();
	isUpdate = true;
	isActivity = true;

	isHandle = true;
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

ZommerGravity Zommer::getGravity()
{
	return this->gravity;
}

void Zommer::reInit()
{
	this->velocity = VECTOR2(0, 0);

	this->isActivity = true;
	isUpdate = true;
	isHandle = true;
	this->setPosition(startPosition);

	this->anim->setPause(false);

	IBonusable::reInit();
	IExplosible::reInit();
	health = 2;


	zommer_direction = ZommerDirection::LEFT_DIRECTION;
	gravity = ZommerGravity::GRAVITY_BOTTOM;

	this->sprite->setRotate(0);
	sprite->setData(reset);
	
}
bool Zommer::getHandle()
{
	return this->isHandle;
}
void Zommer::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - ZOMMER_COLLISION *0.5f + ZOMMER_OFFSET_COLLISION;
	rect.right = position.x + ZOMMER_COLLISION*0.5f - ZOMMER_OFFSET_COLLISION;
	rect.top = position.y + ZOMMER_COLLISION*0.5f - ZOMMER_OFFSET_COLLISION;
	rect.bottom = position.y - ZOMMER_COLLISION*0.5f + ZOMMER_OFFSET_COLLISION;

	this->boundCollision = rect;
}
void Zommer::setStartBound(MetroidRect rect)
{
	this->startBound = rect;
}
void Zommer::handleVelocity(float dt)
{
	dt = 1.0f / 60;
	if (isHandle && !this->isCold&&isActivity)
	{
		switch (gravity)
		{
		case ZommerGravity::GRAVITY_RIGHT:
			this->velocity.x = ZOMMER_VELOCITY_X;
			if (zommer_direction == ZommerDirection::TOP_DIRECTION)
			{
				this->velocity.y = ZOMMER_VELOCITY_Y;
			}
			else
			{
				this->velocity.y = -ZOMMER_VELOCITY_Y;
			}

			break;
		case ZommerGravity::GRAVITY_LEFT:
			this->velocity.x = -ZOMMER_VELOCITY_X;
			if (zommer_direction == ZommerDirection::TOP_DIRECTION)
			{
				this->velocity.y = ZOMMER_VELOCITY_Y;
			}
			else
			{
				this->velocity.y = -ZOMMER_VELOCITY_Y;
			}
			break;
		case ZommerGravity::GRAVITY_TOP:
			this->velocity.y = ZOMMER_VELOCITY_X;
			if (zommer_direction == ZommerDirection::RIGHT_DIRECTION)
			{
				this->velocity.x = ZOMMER_VELOCITY_Y;
			}
			else
			{
				this->velocity.x = -ZOMMER_VELOCITY_Y;
			}
			break;
		case ZommerGravity::GRAVITY_BOTTOM:
			this->velocity.y = -ZOMMER_VELOCITY_X;
			if (zommer_direction == ZommerDirection::RIGHT_DIRECTION)
			{
				this->velocity.x = ZOMMER_VELOCITY_Y;
			}
			else
			{
				this->velocity.x = -ZOMMER_VELOCITY_Y;
			}
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			gravity_bool[i] = false;
		}
	}
}



void Zommer::onCollision(float dt)
{
	dt = 1.0f / 60;

	if (isHandle && !this->isCold&&isActivity)
	{
		for (auto i = this->listWallCanCollide->begin(); i != this->listWallCanCollide->end(); i++)
		{
			BaseObject* x = (*i).second;
			Collision::getInstance()->checkCollision(this, x, dt);

		}
		MetroidRect bound;
		for (auto x = this->listCollide->begin(); x != this->listCollide->end(); x++)
		{

			switch (x->direction)
			{

			case CollideDirection::LEFT:
			{
				if ((gravity == ZommerGravity::GRAVITY_TOP&&this->getBoundCollision().top < x->object->getBoundCollision().bottom)
					|| (gravity == ZommerGravity::GRAVITY_BOTTOM&&this->getBoundCollision().bottom > x->object->getBoundCollision().top))
				{
					break;
				}


				positionCollide = x->object->getBoundCollision();

				this->velocity.x = 0;
				switch (gravity)
				{
				case ZommerGravity::GRAVITY_BOTTOM:

					if (!this->gravity_bool[ZommerGravity::GRAVITY_RIGHT] && zommer_direction == ZommerDirection::RIGHT_DIRECTION)
					{
						gravity = ZommerGravity::GRAVITY_RIGHT;

						zommer_direction = ZommerDirection::TOP_DIRECTION;
						this->sprite->setRotate(270);
					}


					break;
				case ZommerGravity::GRAVITY_TOP:
					if (!this->gravity_bool[ZommerGravity::GRAVITY_RIGHT] && zommer_direction == ZommerDirection::RIGHT_DIRECTION)
					{
						gravity = ZommerGravity::GRAVITY_RIGHT;

						zommer_direction = ZommerDirection::BOTTOM_DIRECTION;
						this->sprite->setRotate(270);
					}

					break;
				}
				gravity_bool[gravity] = true;


				break;
			}
			case CollideDirection::RIGHT:
			{
				if ((gravity == ZommerGravity::GRAVITY_TOP&&this->getBoundCollision().top < x->object->getBoundCollision().bottom)
					|| (gravity == ZommerGravity::GRAVITY_BOTTOM&&this->getBoundCollision().bottom > x->object->getBoundCollision().top))
				{
					break;
				}


				positionCollide = x->object->getBoundCollision();

				this->velocity.x = 0;

				switch (gravity)
				{
				case ZommerGravity::GRAVITY_BOTTOM:
					if (!this->gravity_bool[ZommerGravity::GRAVITY_LEFT] && zommer_direction == ZommerDirection::LEFT_DIRECTION)
					{
						gravity = ZommerGravity::GRAVITY_LEFT;

						zommer_direction = ZommerDirection::TOP_DIRECTION;

						this->sprite->setRotate(90);
					}

					break;
				case ZommerGravity::GRAVITY_TOP:
					if (!this->gravity_bool[ZommerGravity::GRAVITY_LEFT] && zommer_direction == ZommerDirection::LEFT_DIRECTION)
					{
						gravity = ZommerGravity::GRAVITY_LEFT;

						zommer_direction = ZommerDirection::BOTTOM_DIRECTION;
						this->sprite->setRotate(90);

					}

					break;
				}
				gravity_bool[gravity] = true;


				break;
			}
			case CollideDirection::TOP:
			{
				if ((gravity == ZommerGravity::GRAVITY_LEFT&&this->getBoundCollision().left > x->object->getBoundCollision().right)
					|| (gravity == ZommerGravity::GRAVITY_RIGHT&&this->getBoundCollision().right < x->object->getBoundCollision().left))
				{
					break;
				}

				positionCollide = x->object->getBoundCollision();

				this->velocity.y = 0;


				switch (gravity)
				{
				case ZommerGravity::GRAVITY_RIGHT:
					if (!this->gravity_bool[ZommerGravity::GRAVITY_BOTTOM] && zommer_direction == ZommerDirection::BOTTOM_DIRECTION)
					{
						gravity = ZommerGravity::GRAVITY_BOTTOM;

						zommer_direction = ZommerDirection::LEFT_DIRECTION;
						this->sprite->setRotate(0);
					}

					break;
				case ZommerGravity::GRAVITY_LEFT:
					if (!this->gravity_bool[ZommerGravity::GRAVITY_BOTTOM] && zommer_direction == ZommerDirection::BOTTOM_DIRECTION)
					{
						gravity = ZommerGravity::GRAVITY_BOTTOM;

						zommer_direction = ZommerDirection::RIGHT_DIRECTION;
						this->sprite->setRotate(0);
					}

					break;
				}
				gravity_bool[gravity] = true;

				break;
			}
			case CollideDirection::BOTTOM:
			{
				if ((gravity == ZommerGravity::GRAVITY_LEFT&&this->getBoundCollision().left > x->object->getBoundCollision().right)
					|| (gravity == ZommerGravity::GRAVITY_RIGHT&&this->getBoundCollision().right < x->object->getBoundCollision().left))
				{
					break;
				}


				this->velocity.y = 0;
				positionCollide = x->object->getBoundCollision();


				switch (gravity)
				{
				case ZommerGravity::GRAVITY_RIGHT:
					if (!this->gravity_bool[ZommerGravity::GRAVITY_TOP] && zommer_direction == ZommerDirection::TOP_DIRECTION)
					{
						gravity = ZommerGravity::GRAVITY_TOP;

						zommer_direction = ZommerDirection::LEFT_DIRECTION;

						this->sprite->setRotate(180);
					}

					break;
				case ZommerGravity::GRAVITY_LEFT:
					if (!this->gravity_bool[ZommerGravity::GRAVITY_TOP] && zommer_direction == ZommerDirection::TOP_DIRECTION)
					{
						gravity = ZommerGravity::GRAVITY_TOP;

						zommer_direction = ZommerDirection::RIGHT_DIRECTION;

						this->sprite->setRotate(180);
					}
					break;
				}
				gravity_bool[gravity] = true;

				break; 
			}
			}
		}

		this->listCollide->clear();

		
		switch (zommer_direction)
		{
		case ZommerDirection::LEFT_DIRECTION:
		
			if (boundCollision.left + velocity.x*dt < Camera::getInstance()->getBound().left
				&&boundCollision.right  < Camera::getInstance()->getBound().left)
			{

				this->zommer_direction = ZommerDirection::RIGHT_DIRECTION;
				this->velocity.x = 0;

			}
			

			break;
	
		case ZommerDirection::RIGHT_DIRECTION:
		
			if (boundCollision.right + velocity.x*dt > Camera::getInstance()->getBound().right
				&&boundCollision.left > Camera::getInstance()->getBound().right)
			{

				this->zommer_direction = ZommerDirection::LEFT_DIRECTION;
				this->velocity.x = 0;
						
			}
			break;
		
		}

	}

	
}
void Zommer::update(float dt)
{
	dt = 1.0f / 60;

	if (isHandle)
	{
		if (this->isCold)
		{
			timeReturnNormal += dt;
			if(timeReturnNormal >= TIME_RETURN_NOMAL)
			{
				this->anim->setPause(false);
				this->isCold = false;
			}
			else
			{
				this->sprite->setData(this->frameID[anim->getCurrentFrame()]);
				//this->anim->setPause(true);
				this->setVelocity(VECTOR2(0, 0));
				return;
			}
			
		}

		if (!gravity_bool[gravity])
		{
			isUpdate = false;
			switch (gravity)
			{
			case ZommerGravity::GRAVITY_LEFT:
				this->setPosition(VECTOR2(positionCollide.right + ZOMMER_COLLISION *0.5f - 1, positionCollide.bottom - ZOMMER_COLLISION *0.5f));
				if (zommer_direction == ZommerDirection::TOP_DIRECTION)
				{
					gravity = ZommerGravity::GRAVITY_BOTTOM;
					this->sprite->setRotate(0);
					this->setPositionY(positionCollide.top + ZOMMER_COLLISION *0.5f);
				}
				else
				{

					gravity = ZommerGravity::GRAVITY_TOP;
					this->sprite->setRotate(180);
					this->setPositionY(positionCollide.bottom - ZOMMER_COLLISION *0.5f);
				}

				zommer_direction = ZommerDirection::LEFT_DIRECTION;
				break;
			case ZommerGravity::GRAVITY_RIGHT:
				this->setPosition(VECTOR2(positionCollide.left - ZOMMER_COLLISION *0.5f + 1, this->getPosition().y + this->velocity.y*dt));
				if (zommer_direction == ZommerDirection::TOP_DIRECTION)
				{
					gravity = ZommerGravity::GRAVITY_BOTTOM;
					this->sprite->setRotate(0);
					this->setPositionY(positionCollide.top + ZOMMER_COLLISION *0.5f);
				}
				else
				{
					gravity = ZommerGravity::GRAVITY_TOP;
					this->sprite->setRotate(180);
					this->setPositionY(positionCollide.bottom - ZOMMER_COLLISION *0.5f);
				}

				zommer_direction = ZommerDirection::RIGHT_DIRECTION;
				break;
			case ZommerGravity::GRAVITY_TOP:
				this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, positionCollide.bottom - ZOMMER_COLLISION *0.5f + 1));
				if (zommer_direction == ZommerDirection::RIGHT_DIRECTION)
				{
					gravity = ZommerGravity::GRAVITY_LEFT;
					this->setPositionX(positionCollide.right + ZOMMER_COLLISION *0.5f);
					this->sprite->setRotate(90);
				}
				else
				{
					gravity = ZommerGravity::GRAVITY_RIGHT;
					this->sprite->setRotate(270);
					this->setPositionX(positionCollide.left - ZOMMER_COLLISION *0.5f);
				}

				zommer_direction = ZommerDirection::TOP_DIRECTION;
				break;
			case ZommerGravity::GRAVITY_BOTTOM:
				this->setPosition(VECTOR2(this->positionCollide.left - ZOMMER_COLLISION *0.5f, positionCollide.top + ZOMMER_COLLISION *0.5f - 1));
				if (zommer_direction == ZommerDirection::RIGHT_DIRECTION)
				{
					gravity = ZommerGravity::GRAVITY_LEFT;
					this->setPositionX(this->positionCollide.right + ZOMMER_COLLISION *0.5f);
					this->sprite->setRotate(90);
				}
				else
				{
					gravity = ZommerGravity::GRAVITY_RIGHT;
					this->setPositionX(this->positionCollide.left - ZOMMER_COLLISION *0.5f);
					this->sprite->setRotate(270);
				}

				zommer_direction = ZommerDirection::BOTTOM_DIRECTION;
				break;
			default:
				break;
			}
		}


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

					Sound::getInstance()->play(SOUND_EXPLOSION, false);
					IExplosible::start();
					this->setVelocity(VECTOR2(0, 0));
					//this->isActivity = false;
					this->isHandle = false;
					this->isCold = false;
				}
			}
		}

		if (isUpdate)
		{
			this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt));

		}
		setBoundCollision();
		
		this->anim->update(dt);
		isUpdate = true;
	}

	if (!Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->startBound)
		&& !Collision::getInstance()->isCollide(Camera::getInstance()->getBound(), this->boundCollision))
	{
		reInit();
		//isActivity = false;
		setBoundCollision();
		gravity_bool[gravity] = true;
	}
	/*else
	{
		isActivity = true;
	}*/
	if(isExplose)
	{
		IBonusable::update(dt);
	}
	else
	{
		IExplosible::update(dt);
		if (isExplose)
		{
			
 			IBonusable::start(samus->getRocket());
		}
	}

	
	
	
}

void Zommer::draw()
{
	if(this->canDraw)
	{
		this->sprite->draw();
	}
	
}

void Zommer::setStartPosition(VECTOR2 position)
{
	this->startPosition = position;
}

void Zommer::setBeHit(bool hit)
{
	this->beHit = hit;
}
void Zommer::decreaseHealth(float dame)
{
	this->health = this->health - dame;
}



map<int, BaseObject*>* Zommer::getListWallCanCollide()
{
	return this->listWallCanCollide;
}
list<CollisionReturn>* Zommer::getListCollide()
{
	return this->listCollide;
}

