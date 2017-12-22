#include "Zommer.h"

#define TIME_FRAME_DELAY 0.15f
#define ZOMMER_VELOCITY_X 45
#define ZOMMER_VELOCITY_Y 45
#define ZOMMER_OFFSET_COLLISION 0.1f
Zommer::Zommer(TextureManager * textureM, Graphics * graphics, EnemyColors color) : BaseObject(eID::ZOMMER),IFreezable(IndexManager::getInstance()->zoomerBlue)
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

	//this->topSide=()
	this->setOrigin(VECTOR2(0.5, 0.5));
	
	this->listWallCanCollide = new list<BaseObject*>();
	this->listCollide = new list<CollisionReturn>();
	anim->start();

	//this->setPosition(VECTOR2(679, 1367));

	zommer_direction = ZommerDirection::RIGHT_DIRECTION;
	gravity = ZommerGravity::GRAVITY_BOTTOM;



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
	for (size_t i = 0; i < 4; i++)
	{
		gravity_bool[i] = false;
	}
	setBoundCollision();
	isUpdate = true;
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

void Zommer::onCollision(float dt)
{
	for (auto i = this->getListWallCanCollide()->begin(); i != this->getListWallCanCollide()->end(); i++)
	{
		Collision::getInstance()->checkCollision(this, *i, dt);
	}
	MetroidRect bound;
	for (auto x = this->listCollide->begin(); x != this->listCollide->end(); x++)
	{

		switch (x->direction)
		{
			if(x->object->getId()==PORT)
			{
				int test = 0;
			}
		case CollideDirection::LEFT:

			bound = Collision::getInstance()->getSweptBroadphaseRect(this->boundCollision, VECTOR2(velocity.x, 0), dt);
			if (Collision::getInstance()->isCollide(bound, x->object->getBoundCollision()))
			{
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
			}

			break;
		case CollideDirection::RIGHT:


			bound = Collision::getInstance()->getSweptBroadphaseRect(this->boundCollision, VECTOR2(velocity.x, 0), dt);

			if (Collision::getInstance()->isCollide(bound, x->object->getBoundCollision()))
			{
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
			}

			break;
		case CollideDirection::TOP:
			bound = Collision::getInstance()->getSweptBroadphaseRect(this->boundCollision, VECTOR2(0, velocity.y), dt);

			if (Collision::getInstance()->isCollide(bound, x->object->getBoundCollision()))
			{
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
			}
			break;
		case CollideDirection::BOTTOM:
			bound = Collision::getInstance()->getSweptBroadphaseRect(this->boundCollision, VECTOR2(0, velocity.y), dt);

			if (Collision::getInstance()->isCollide(bound, x->object->getBoundCollision()))
			{
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
			}
			break;
		}
	}

	this->listCollide->clear();
}
void Zommer::update(float dt)
{

	if(this->isCold)
	{
		this->sprite->setData(this->frameID[anim->getCurrentFrame()]);
		this->anim->setPause(true);
		return;
	}
	else
	{
		this->anim->setPause(false);
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
	if (isUpdate)
	{
		this->setPosition(VECTOR2(this->getPosition().x + this->velocity.x*dt, this->getPosition().y + this->velocity.y*dt));

	}

	setBoundCollision();
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

	for (size_t i = 0; i < 4; i++)
	{
		gravity_bool[i] = false;
	}

	
	this->anim->update(dt);
	isUpdate = true;
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

