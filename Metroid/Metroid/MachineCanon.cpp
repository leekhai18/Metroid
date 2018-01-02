#include "MachineCanon.h"

#define TIME_TO_FIRE 0.1f
#define OFFSET 3
#define FRAME_DELAY_ANIMATON 0.3f
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
	bullet = new CanonBullet(graphics, textureM);
	this->type = type;
	
	switch (type)
	{
	case CanonType::CANON_LEFT:
	
		anim = new Animation(sprite, IndexManager::getInstance()->canonTypeLeft, 5, FRAME_DELAY_ANIMATON);
		this->setPositionY(this->getPosition().y + OFFSET);
		sideToFire = IndexManager::getInstance()->canonRightDown;
		listFrame = IndexManager::getInstance()->canonTypeLeft;

		this->sprite->setData(listFrame[0]);
		break;
	case CanonType::CANON_RIGHT:
	
		anim = new Animation(sprite, IndexManager::getInstance()->canonTypeRight, 5, FRAME_DELAY_ANIMATON);
		this->setPositionY(this->getPosition().y + OFFSET);
		sideToFire = IndexManager::getInstance()->canonLeftDown;
		listFrame = IndexManager::getInstance()->canonTypeRight;
		this->sprite->setData(listFrame[0]);
		break;
	case CanonType::CANON_TOP:
	
		anim = new Animation(sprite, IndexManager::getInstance()->canonTypeTop, 5, FRAME_DELAY_ANIMATON);
		sideToFire = IndexManager::getInstance()->canonDown;
		listFrame = IndexManager::getInstance()->canonTypeTop;
		this->setPosition(VECTOR2(this->getPosition().x+ 30, this->getPosition().y + 16 * 0.5 + 4));
		this->sprite->setData(listFrame[0]);
		break;
	default:
		break;
	}
	anim->start();
	stop = false;
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
	if(!isFire)
	{
		bullet->handleVelocity(dt);
	}
	
}

void MachineCanon::onCollision(float dt)
{
	bullet->onCollision(dt);
}

void MachineCanon::update(float dt)
{
	//this->sprite->setData(listFrame[0]); 
	//if (type == CanonType::CANON_TOP)
	//{
	//	this->setPosition(VECTOR2(this->startPosition.x + 3, startPosition.y  +2 ));
	//	int test = 0;
	//}
	

	anim->update(dt);
	
	CanonDirection canonDirection = static_cast<CanonDirection>(listFrame[anim->getCurrentFrame()]);
	switch (canonDirection)
	{
	case CanonDirection::DUP:
		this->setPosition(VECTOR2(startPosition.x , startPosition.y + OFFSET +0.5f));
		//this->setPositionY(startPosition.y + OFFSET );
		break;
	case CanonDirection::DDOWN:	
		this->setPosition(VECTOR2(startPosition.x , startPosition.y - OFFSET - 0.5f));
		//this->setPositionY(startPosition.y - OFFSET);
		break;
	case CanonDirection::DLEFT_DOWN:	
		this->setPosition(VECTOR2(startPosition.x - OFFSET*0.5f , startPosition.y - OFFSET*0.5f ));
		break;
	case CanonDirection::DRIGHT_DOWN:
	
		this->setPosition(VECTOR2(startPosition.x + OFFSET*0.5f, startPosition.y - OFFSET*0.5f));
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
}
MachineCanon::~MachineCanon()
{
	delete listWallCanCollide;
	delete sprite;
}
