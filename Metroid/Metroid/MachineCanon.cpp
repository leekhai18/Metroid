#include "MachineCanon.h"

#define TIME_TO_FIRE 0.1f
#define OFFSET 3
#define FRAME_DELAY_ANIMATON 0.6f
MachineCanon::MachineCanon()
{
}



MachineCanon::MachineCanon(Graphics * graphics, TextureManager * textureM, Samus * samus, CanonType type):BaseObject(eID::MACHINE_CANON)
{
	this->sprite = new Sprite();
	if (!this->sprite->initialize(graphics, textureM, SpriteManager::getInstance()))
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "Can not init sprite Zommer");
	}

	this->listWallCanCollide = new map<int, BaseObject*>();
	this->samus = samus;
	bullet = new CanonBullet(graphics, textureM, type,this);
	this->type = type;
	
	switch (type)
	{
	case CanonType::CANON_LEFT:
	
		anim = new Animation(sprite, IndexManager::getInstance()->canonTypeLeft, 5, FRAME_DELAY_ANIMATON);
		this->setPositionY(this->getPosition().y + OFFSET);
		sideToFire = IndexManager::getInstance()->canonRightDown;
		listFrame = IndexManager::getInstance()->canonTypeLeft;
		directionBefore = CanonDirection::DDOWN;
		this->sprite->setData(listFrame[0]);
		break;
	case CanonType::CANON_RIGHT:
	
		anim = new Animation(sprite, IndexManager::getInstance()->canonTypeRight, 5, FRAME_DELAY_ANIMATON);
		this->setPositionY(this->getPosition().y + OFFSET);
		sideToFire = IndexManager::getInstance()->canonLeftDown;
		listFrame = IndexManager::getInstance()->canonTypeRight;
		directionBefore = CanonDirection::DDOWN;
		this->sprite->setData(listFrame[0]);
		break;
	case CanonType::CANON_TOP:
	
		anim = new Animation(sprite, IndexManager::getInstance()->canonTypeTop, 5, FRAME_DELAY_ANIMATON);
		sideToFire = IndexManager::getInstance()->canonDown;
		listFrame = IndexManager::getInstance()->canonTypeTop;
		this->setPosition(VECTOR2(this->getPosition().x+ 30, this->getPosition().y + 16 * 0.5 + 4));
		this->sprite->setData(listFrame[0]);
		directionBefore = CanonDirection::DRIGHT;
		break;
	default:
		break;
	}
	anim->start();
	
	timeDelayFire = 0;
	this->sprite->setOrigin(VECTOR2(0.5, 0.5));
}
void MachineCanon::reInit()
{
	switch (type)
	{
	case CanonType::CANON_LEFT:
		bullet->setPosition(VECTOR2(boundCollision.right,boundCollision.bottom));

		break;
	case CanonType::CANON_RIGHT:
		bullet->setPosition(VECTOR2(boundCollision.left, boundCollision.bottom));
		break;
	case CanonType::CANON_TOP:
		bullet->setPosition(VECTOR2(boundCollision.right - this->getSprite()->getWidth() *0.5f, boundCollision.bottom));
		break;
	default:
		break;
	}
	this->bullet->setListWallCanCollide(this->listWallCanCollide);
}

void MachineCanon::setStartPosition(VECTOR2 position)
{
	this->startPosition = position;
}

void MachineCanon::setBoundCollision()
{
	MetroidRect rect;
	VECTOR2 position(this->getPosition().x, this->getPosition().y);
	rect.left = position.x - 20 *0.5f;
	rect.right = position.x + 20*0.5f ;
	rect.top = position.y + 20*0.5f ;
	rect.bottom = position.y - 20*0.5f ;

	this->boundCollision = rect;
}


void MachineCanon::handleVelocity(float dt)
{

	if(isFire)
	{
		bullet->handleVelocity(dt);
		bullet->setListWallCanCollide(this->listWallCanCollide);
		
		if(bullet->getCollided())
		{
			this->anim->setPause(false);
			this->anim->nextFrame();
			isFire = false;
		}
	}
	
}

void MachineCanon::onCollisionSamus(Samus* samus,float dt)
{
	if(isActivity)
	{
		
	}
	bullet->onCollisionSamus(samus, dt);
}

void MachineCanon::onCollision(float dt)
{
	bullet->onCollision(dt);
}

void MachineCanon::update(float dt)
{

	anim->update(dt);
	bullet->update(dt);
	CanonDirection canonDirection = static_cast<CanonDirection>(listFrame[anim->getCurrentFrame()]);
	switch (canonDirection)
	{
	case CanonDirection::DUP:
		this->setPosition(VECTOR2(startPosition.x , startPosition.y + OFFSET +0.5f));
		//directionBefore
		//this->setPositionY(startPosition.y + OFFSET );
		break;
	case CanonDirection::DDOWN:	
		this->setPosition(VECTOR2(startPosition.x , startPosition.y - OFFSET - 0.5f));
		if (type == CanonType::CANON_TOP)
		{
			if (!isFire)
			{
				bullet->init();
				isFire = true;
				this->anim->setPause(true);			
			}
			
		}
		//this->setPositionY(startPosition.y - OFFSET);
		break;
	case CanonDirection::DLEFT_DOWN:	
		this->setPosition(VECTOR2(startPosition.x - OFFSET*0.5f , startPosition.y - OFFSET*0.5f ));
		if(type == CanonType::CANON_RIGHT)
		{
			if (!isFire)
			{
				bullet->init();
				isFire = true;
				this->anim->setPause(true);
			}
			
		}
		break;
	case CanonDirection::DRIGHT_DOWN:
	
		this->setPosition(VECTOR2(startPosition.x + OFFSET*0.5f, startPosition.y - OFFSET*0.5f));
		if (type == CanonType::CANON_LEFT)
		{
			if (!isFire)
			{
				bullet->init();
				isFire = true;
				this->anim->setPause(true);
			}
			//bullet->init();
		}
		break;
	case CanonDirection::DLEFT_UP:
		this->setPosition(VECTOR2(startPosition.x - OFFSET*0.5f, startPosition.y + OFFSET*0.5f));
		break;
	case CanonDirection::DRIGHT_UP:
		this->setPosition(VECTOR2(startPosition.x + OFFSET*0.5f, startPosition.y + OFFSET*0.5f));
		break;
	case CanonDirection::DRIGHT:
		this->setPosition(VECTOR2(startPosition.x + OFFSET, startPosition.y));
		break;
	case CanonDirection::DLEFT:
		this->setPosition(VECTOR2(startPosition.x - OFFSET, startPosition.y));
		break;
	default:
		break;
	}
	
}

void MachineCanon::draw()
{
	sprite->draw();
	bullet->draw();
}
map<int, BaseObject*>* MachineCanon::getListCanCollide()
{
	return this->listWallCanCollide;
}
MachineCanon::~MachineCanon()
{
	delete listWallCanCollide;
	delete sprite;
}
