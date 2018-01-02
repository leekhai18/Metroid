#include "ObjectManager.h"
#include "GameLog.h"
#include "MaruMari.h"
#include "Bomb.h"
#include "EnergyTank.h"
#include "IceBeam.h"
#include "LongBeam.h"
#include "MissileRocket.h"
#include "Varia.h"
#include "BossKraid.h"
#include "BossDragon.h"
#include "AlienBig.h"
#include "AlienSmall.h"
#include "GateBlue.h"
#include "GateRed.h"
#include "Port.h"
#include "Brick.h"
#include "Zommer.h"
#include "Zeb.h"
#include "Waver.h"
#include "Skree.h"
#include "Ripper.h"
#include "Rio.h"
#include "MotherBrain.h"
#include "Camera.h"
#include "Collision.h"
#include "BossKraid.h"
#include "MachineCanon.h"
#include "rapidjson-master\include\rapidjson\writer.h"
#include "rapidjson-master\include\rapidjson\ostreamwrapper.h"


#define TIME_RETRIEVE 0.65f

ObjectManager* ObjectManager::instance = nullptr;

ObjectManager * ObjectManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new ObjectManager();
	}

	return instance;
}
ObjectManager::ObjectManager()
{
	this->listNotWallCanCollideSamus = new map<int, BaseObject*>();
	this->listObjectNotWallOnViewPort = new map<int, BaseObject*>();
	this->listWallCanCollideSamus = new map<int, BaseObject*>();
	this->listWallEmnermy = new map<int, BaseObject*>();
	this->bullet_object = new map<int, BaseObject*>();
	this->totalObjectsPerFrame = 0;
	this->timer = 0;
}
void ObjectManager::release()
{

	for (auto i = map_object.begin(); i != map_object.end(); i++)
	{
		delete (*i).second;
	}
	delete listObjectNotWallOnViewPort;
	delete listNotWallCanCollideSamus;

	delete listWallCanCollideSamus;
	delete listWallEmnermy;
	delete bullet_object;

	quadtree->clear();
	delete quadtree;

	delete instance;
}


void ObjectManager::handleVelocity(float dt)
{

	timer += dt;

	if (timer >= TIME_RETRIEVE)
	{
		timer = 0;
		listNotWallCanCollideSamus->clear();
		listObjectNotWallOnViewPort->clear();
		listWallCanCollideSamus->clear();
		MetroidRect r = Camera::getInstance()->getBound();

		//Get all objects that can collide with current obj
		quadtree->retrieve(listNotWallCanCollideSamus, listObjectNotWallOnViewPort, listWallCanCollideSamus, MetroidRect(r.top + 20, r.bottom - 40, r.left - 50, r.right + 50), samus);
		//quadtree->retrieve(listNotWallCanCollideSamus, listObjectNotWallOnViewPort, listWallCanCollideSamus, MetroidRect(r.top + 40, r.bottom - 40, r.left - 40, r.right + 40), samus);
	}


	for (map<int,BaseObject*>::iterator i = listObjectNotWallOnViewPort->begin(); i != listObjectNotWallOnViewPort->end(); ++i)
	{
		switch (((*i).second)->getId())
		{
		case eID::WAVER:
		{
			Waver* waver = static_cast<Waver*>((*i).second);
			waver->handleVelocity(dt);
			break;
		}
		case eID::ZOMMER:
		{
			Zommer* zommer = static_cast<Zommer*>((*i).second);
			zommer->handleVelocity(dt);
			break;
		}
		case eID::BOSSKRAID:
		{
			BossKraid* bossKraid = static_cast<BossKraid*>((*i).second);
			bossKraid->handleVelocity(dt);
			break;
		}
		case eID::MOTHERBRAIN:
		{
			MotherBrain* motherBrain = static_cast<MotherBrain*>((*i).second);
			motherBrain->handleVelocity(dt);
			break;
		}
		case eID::ZEB:
		{
			Zeb* zeb = static_cast<Zeb*>((*i).second);
			zeb->handleVelocity(dt);
			break;
		}
		case eID::RIPPER:
		{
			Ripper* ripper = static_cast<Ripper*>((*i).second);
			ripper->handleVelocity(dt);
			break;
		}
		case eID::RIO:
		{
			Rio* rio = static_cast<Rio*>((*i).second);
			rio->setTarget(samus->getPosition());
			rio->handleVelocity(dt);
			break;
		}
		case eID::SKREE:
		{
			Skree* skr = static_cast<Skree*>((*i).second);
			skr->setTarget(samus->getPosition());
			skr->handleVelocity(dt);
			break;
		}
		case eID::MACHINE_CANON:
		{
			MachineCanon* canon = static_cast<MachineCanon*>((*i).second);
			
			canon->handleVelocity(dt);
			break;
		}
		default:
			break;
		}
	}

}

void ObjectManager::onCheckCollision(float dt)
{

	if (listObjectNotWallOnViewPort)
	{
		for (map<int,BaseObject*>::iterator i = listObjectNotWallOnViewPort->begin(); i != listObjectNotWallOnViewPort->end(); ++i)
		{
			/*for (map<int, BaseObject*>::iterator x = listWallEmnermy->begin(); x != listWallEmnermy->end(); ++x)
			{
				Collision::getInstance()->checkCollision((*i).second, (*x).second, dt);
			}	*/	
			((*i).second)->onCollision(dt);
		}
	}

//we must check boom collision first because samus can collide with wall when boom change velocity of samus
#pragma region handle Boom
	//get list collide
	for (auto x = listNotWallCanCollideSamus->begin(); x != listNotWallCanCollideSamus->end(); x++)
	{
		BaseObject* object = (*x).second;

		for (unsigned i = 0; i < BoomBombPool::getInstance()->getListUsing().size(); i++)
		{
			if (Collision::getInstance()->isCollide(BoomBombPool::getInstance()->getListUsing().at(i)->getBoundCollision(), object->getBoundCollision()))
			{
				BoomBombPool::getInstance()->getListUsing().at(i)->getListCollide()->push_back(object);
			}
		}
		for (unsigned i = 0; i < BulletPool::getInstance()->getListUsing().size(); i++)
			Collision::getInstance()->checkCollision(BulletPool::getInstance()->getListUsing().at(i), object, dt);

		for (unsigned i = 0; i < RocketPool::getInstance()->getListUsing().size(); i++)
			Collision::getInstance()->checkCollision(RocketPool::getInstance()->getListUsing().at(i), object, dt);

	}
	//handle collide
	for (unsigned i = 0; i < BulletPool::getInstance()->getListUsing().size(); i++)
		BulletPool::getInstance()->getListUsing().at(i)->onCollision();

	for (unsigned i = 0; i < RocketPool::getInstance()->getListUsing().size(); i++)
		RocketPool::getInstance()->getListUsing().at(i)->onCollision();

	for (unsigned i = 0; i < BoomBombPool::getInstance()->getListUsing().size(); i++)
		BoomBombPool::getInstance()->getListUsing().at(i)->onCollision();
#pragma endregion


#pragma region handle Wall
	for (auto x = listWallCanCollideSamus->begin(); x != listWallCanCollideSamus->end(); x++)
	{
		BaseObject* object = (*x).second;
		samus->setListCanCollide(listWallCanCollideSamus);
		Collision::getInstance()->checkCollision(samus, object, dt);

		for (unsigned i = 0; i < BulletPool::getInstance()->getListUsing().size(); i++)
			Collision::getInstance()->checkCollision(BulletPool::getInstance()->getListUsing().at(i), object, dt);

		for (unsigned i = 0; i < RocketPool::getInstance()->getListUsing().size(); i++)
			Collision::getInstance()->checkCollision(RocketPool::getInstance()->getListUsing().at(i), object, dt);
	}
	// handle on listCollide
	samus->onCollision(dt);
	for (unsigned i = 0; i < BulletPool::getInstance()->getListUsing().size(); i++)
		BulletPool::getInstance()->getListUsing().at(i)->onCollision();
	for (unsigned i = 0; i < RocketPool::getInstance()->getListUsing().size(); i++)
		RocketPool::getInstance()->getListUsing().at(i)->onCollision();
#pragma endregion


#pragma region handle Other wall
	// Get listCollide
	for (auto x = listNotWallCanCollideSamus->begin(); x != listNotWallCanCollideSamus->end(); x++)
	{
		BaseObject* object = (*x).second;
		//if (object->getId() == BOSSKRAID)
		//{
		//	int test = 0;
		//}
		samus->setListCanCollide(listNotWallCanCollideSamus);
		Collision::getInstance()->checkCollision(samus, object, dt);

		if (object->getId() == eID::SKREE)
		{
			Skree* skr = static_cast<Skree*>(object);

			if (skr->checkCollision(samus, dt))
			{
				skr->onCollision(samus);
			}
		}

		if (object->getId() == eID::MOTHERBRAIN)
		{
			MotherBrain* motherBrain = static_cast<MotherBrain*>(object);

			motherBrain->onCollision(samus, dt);
		}
	}

	// handle on listCollide
	samus->onCollision(dt);


#pragma endregion

}

void ObjectManager::onCheckCollision(BaseObject * obj, float frametime)
{

}

int ObjectManager::getTotalObjectsPerFrame()
{
	return this->totalObjectsPerFrame;
}

void ObjectManager::setTotalObjectPerFrame(int num)
{
	this->totalObjectsPerFrame = num;
}

void ObjectManager::update(float dt)
{
	if (listObjectNotWallOnViewPort)
	{
		for (auto i = listObjectNotWallOnViewPort->begin(); i != listObjectNotWallOnViewPort->end(); ++i)
		{
			BaseObject* object = (*i).second;
			object->update(dt);
		}
	}
}

void ObjectManager::draw()
{
	if (listObjectNotWallOnViewPort)
	{
		for (auto i = listObjectNotWallOnViewPort->begin(); i != listObjectNotWallOnViewPort->end(); ++i)
		{
			(*i).second->draw();
		}
	}
}
void ObjectManager::insertQuadTreeNode(const Value & value, Quadtree* quadtree)
{
	const Value& levelValue = value["Level"];
	const Value& regionValue = value["Region"];
	const Value& objectList = value["ObjectList"];
	const Value& nodes = value["Nodes"];
	int  level = levelValue.GetInt();

	MetroidRect region;
	region.top = regionValue["Top"].GetFloat();
	region.left = regionValue["Left"].GetFloat();
	region.right = regionValue["Right"].GetFloat();
	region.bottom = regionValue["Bottom"].GetFloat();


	Quadtree* node = new Quadtree(level, region);

	for (SizeType i = 0; i < objectList.Size(); i++)
	{
		int id = objectList[i]["Id"].GetInt();
		//BaseObject* obj = (*map_object.find(id)).second;
		string temp = std::to_string(id);
		const char *pchar = temp.c_str();
		GAMELOG(pchar);
		node->getObjectList().insert(pair<int, BaseObject*>(*map_object.find(id)));
	
	}
	if (!nodes.IsNull())
	{

		node->getNodes() = new list<Quadtree*>();
		for (SizeType i = 0; i < nodes.Size(); i++)
		{
			insertQuadTreeNode(nodes[i], node);
		}
	}
	//quadtree->getNodes() = new list<Quadtree*>();
	quadtree->getNodes()->push_back(node);

}
bool ObjectManager::load_quatree(const char * filename)
{
	try
	{
		ifstream ifs(filename);
		IStreamWrapper isw(ifs);
		Document jSon;
		jSon.ParseStream(isw);

		//quadtree =new Quadtree()
		const Value& levelValue = jSon["Level"];
		const Value& regionValue = jSon["Region"];
		const Value& objectList = jSon["ObjectList"];

		const Value& nodes = jSon["Nodes"];

		int  level = levelValue.GetInt();

		MetroidRect region;
		region.top = regionValue["Top"].GetFloat();
		region.left = regionValue["Left"].GetFloat();
		region.right = regionValue["Right"].GetFloat();
		region.bottom = regionValue["Bottom"].GetFloat();


		quadtree = new Quadtree(level, region);
		quadtree->getNodes() = new list<Quadtree*>();
		if (!nodes.IsNull())
		{
			for (SizeType i = 0; i < nodes.Size(); i++)
			{
				insertQuadTreeNode(nodes[i], quadtree);
			}

		}


	}
	catch (const std::exception&)
	{
		return false;
	}
	return true;
}


void ObjectManager::init(TextureManager * textureM, Graphics * graphics, Samus* samus)
{
	this->textureManager = textureM;
	this->graphics = graphics;
	this->samus = samus;
}

bool ObjectManager::load_list(const char * filename)
{
	try
	{
		ifstream ifs(filename);
		IStreamWrapper isw(ifs);
		Document jSon;
		jSon.ParseStream(isw);
		float x, y, height, width;
		int id;
		MetroidRect bound;

		// write json file
		//Document d;
		//d.Parse("json");
		//ofstream ofs("json\\aaa.json");
		//OStreamWrapper osw(ofs);
		//Writer<OStreamWrapper> writer(osw);

		//writer.StartObject();

		const Value& listMachineGun = jSon["MaChineGun"];
		for (SizeType i = 0; i < listMachineGun.Size(); i++)
		{
			
			
			id = listMachineGun[i]["id"].GetInt();
			x = listMachineGun[i]["x"].GetFloat();
			y = listMachineGun[i]["y"].GetFloat();
			height = listMachineGun[i]["height"].GetFloat();
			width = listMachineGun[i]["width"].GetFloat();
			x =x + 16*0.5 ;
			y =y - 16*0.5  ;
			//int type = listMachineGun[i]["type"].GetInt();
			CanonType type= static_cast<CanonType>(listMachineGun[i]["type"].GetInt());
			MachineCanon *maChineGun = new MachineCanon(graphics, textureManager, samus,type);
			maChineGun->setPosition(VECTOR2(x, y));
			maChineGun->setStartPosition(VECTOR2(x, y));
			/*			writer.StartObject();
			writer.Key("x");
			writer.Double(x);
			writer.Key("y");
			writer.Double(y);
			writer.Key("height");
			writer.Double(height);
			writer.Key("width");
			writer.Double(width);
			writer.EndObject();*/

			
			maChineGun->setBoundCollision();
			maChineGun->reInit();
			maChineGun->setActiveBound(maChineGun->getBoundCollision());


			map_object.insert(std::pair<int, BaseObject*>(id, maChineGun));
		}

		const Value& listDefense = jSon["DefenseBoss"];
		for (SizeType i = 0; i < listDefense.Size(); i++)
		{
			BaseObject *defense = new BaseObject(eID::DEFENSEBOSS);

			id = listDefense[i]["id"].GetInt();
			x = listDefense[i]["x"].GetFloat();
			y = listDefense[i]["y"].GetFloat();
			height = listDefense[i]["height"].GetFloat();
			width = listDefense[i]["width"].GetFloat();

			/*			writer.StartObject();
			writer.Key("x");
			writer.Double(x);
			writer.Key("y");
			writer.Double(y);
			writer.Key("height");
			writer.Double(height);
			writer.Key("width");
			writer.Double(width);
			writer.EndObject();*/

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + width;
			bound.bottom = bound.top - height;
			defense->setBoundCollision(bound);

			defense->setActiveBound(bound);


			map_object.insert(std::pair<int, BaseObject*>(id, defense));
		}

#pragma region Wall
		// Load Wall POS , 650 
		/*	writer.Key("Wall");


		writer.StartArray();*/
		const Value& listWall = jSon["Wall"];

		

			for (SizeType i = 0; i < listWall.Size(); i++)
			{
				BaseObject *wall = new BaseObject(eID::WALL);

				id = listWall[i]["id"].GetInt();
				x = listWall[i]["x"].GetFloat();
				y = listWall[i]["y"].GetFloat();
				height = listWall[i]["height"].GetFloat();
				width = listWall[i]["width"].GetFloat();

				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.Key("height");
				writer.Double(height);
				writer.Key("width");
				writer.Double(width);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + width;
				bound.bottom = bound.top - height;
				wall->setBoundCollision(bound);

				wall->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, wall));
			}
		

		//writer.EndArray();	
#pragma endregion

#pragma region other
		// create Elevator
		/*writer.Key("Elevator");
		writer.StartArray();*/
		const Value& elevator = jSon["Elevator"];
		
			for (SizeType i = 0; i < elevator.Size(); i++)
			{
				BaseObject *elev = new BaseObject(eID::ELEVATOR);

				id = elevator[i]["id"].GetInt();
				x = elevator[i]["x"].GetFloat();
				y = elevator[i]["y"].GetFloat();
				height = elevator[i]["height"].GetFloat();
				width = elevator[i]["width"].GetFloat();

				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.Key("height");
				writer.Double(height);
				writer.Key("width");
				writer.Double(width);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + width;
				bound.bottom = bound.top - height;
				elev->setBoundCollision(bound);

				elev->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, elev));
			}
		
		//writer.EndArray();

		// create Fire
		/*writer.Key("Fire");
		writer.StartArray();*/
		const Value& listFire = jSon["Fire"];
		
			for (SizeType i = 0; i < listFire.Size(); i++)
			{
				BaseObject *fire = new BaseObject(eID::FIRE);

				id = listFire[i]["id"].GetInt();
				x = listFire[i]["x"].GetFloat();
				y = listFire[i]["y"].GetFloat();
				height = listFire[i]["height"].GetFloat();
				width = listFire[i]["width"].GetFloat();

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("height");
				//writer.Double(height);
				//writer.Key("width");
				//writer.Double(width);
				//writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + width;
				bound.bottom = bound.top - height;
				fire->setBoundCollision(bound);

				fire->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, fire));
			}
		
		//writer.EndArray();

#pragma endregion

#pragma region Special Bricks
		// BrickSecretBlue, 2
		//writer.Key("BrickSecretBlue");
		//writer.StartArray();

		const Value& listBrickSecretBlue = jSon["BrickSecretBlue"];
		
			for (SizeType i = 0; i < listBrickSecretBlue.Size(); i++)
			{
				Brick *bsb = new Brick(this->textureManager, this->graphics, BrickStyle::BrickSecretBlue);

				id = listBrickSecretBlue[i]["id"].GetInt();
				x = listBrickSecretBlue[i]["x"].GetFloat();
				y = listBrickSecretBlue[i]["y"].GetFloat();
				bsb->setPosition(VECTOR2(x, y));


				/*		writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + bsb->getSprite()->getWidth();
				bound.bottom = bound.top - bsb->getSprite()->getHeight();
				bsb->setBoundCollision(bound);

				bsb->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, bsb));
			}
		
		//writer.EndArray();

		// BrickSerectGreen, 9
		/*	writer.Key("BrickSerectGreen");
		writer.StartArray();*/

		const Value& listBrickSerectGreen = jSon["BrickSerectGreen"];
		
			for (SizeType i = 0; i < listBrickSerectGreen.Size(); i++)
			{
				Brick *bsg = new Brick(this->textureManager, this->graphics, BrickStyle::BrickSerectGreen);

				id = listBrickSerectGreen[i]["id"].GetInt();
				x = listBrickSerectGreen[i]["x"].GetFloat();
				y = listBrickSerectGreen[i]["y"].GetFloat();
				bsg->setPosition(VECTOR2(x, y));

				/*		writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + bsg->getSprite()->getWidth();
				bound.bottom = bound.top - bsg->getSprite()->getHeight();
				bsg->setBoundCollision(bound);

				bsg->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, bsg));
			}
		
		//writer.EndArray();


		// BrickGreen, 5
		/*writer.Key("BrickGreen");
		writer.StartArray();*/
		const Value& listBrickGreen = jSon["BrickGreen"];
		
			for (SizeType i = 0; i < listBrickGreen.Size(); i++)
			{
				Brick *bg = new Brick(this->textureManager, this->graphics, BrickStyle::BrickGreen);
				id = listBrickGreen[i]["id"].GetInt();

				x = listBrickGreen[i]["x"].GetFloat();
				y = listBrickGreen[i]["y"].GetFloat();
				bg->setPosition(VECTOR2(x, y));

				/*	writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + bg->getSprite()->getWidth();
				bound.bottom = bound.top - bg->getSprite()->getHeight();
				bg->setBoundCollision(bound);

				bg->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, bg));
			}
		
		//writer.EndArray();


		// BrickBlue, 28, có 8 viên visible is false, so you need to check 
		//writer.Key("BrickBlue");
		//writer.StartArray();

		bool isVisible;
		const Value& listBrickBlue = jSon["BrickBlue"];
		
			for (SizeType i = 0; i < listBrickBlue.Size(); i++)
			{
				Brick *bb = new Brick(this->textureManager, this->graphics, BrickStyle::BrickBlue);

				id = listBrickBlue[i]["id"].GetInt();
				x = listBrickBlue[i]["x"].GetFloat();
				y = listBrickBlue[i]["y"].GetFloat();
				bb->setPosition(VECTOR2(x, y));

				isVisible = listBrickBlue[i]["visible"].GetBool();
				bb->setVisible(isVisible);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("visible");
				//writer.Bool(isVisible);
				//writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + bb->getSprite()->getWidth();
				bound.bottom = bound.top - bb->getSprite()->getHeight();
				bb->setBoundCollision(bound);

				bb->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, bb));
			}
		
		//	writer.EndArray();

		// BrickRed, 63
		/*	writer.Key("BrickRed");
		writer.StartArray();*/
		const Value& listBrickRed = jSon["BrickRed"];
	
			for (SizeType i = 0; i < listBrickRed.Size(); i++)
			{
				Brick *br = new Brick(this->textureManager, this->graphics, BrickStyle::BrickRed);

				id = listBrickRed[i]["id"].GetInt();
				x = listBrickRed[i]["x"].GetFloat();
				y = listBrickRed[i]["y"].GetFloat();
				br->setPosition(VECTOR2(x, y));

				/*		writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + br->getSprite()->getWidth();
				bound.bottom = bound.top - br->getSprite()->getHeight();
				br->setBoundCollision(bound);

				br->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, br));
			}
		
		//writer.EndArray();

#pragma endregion

#pragma region Gates and Ports
		// create GateBlue R, 25 
		/*	writer.Key("GateBlueR");
		writer.StartArray();*/
		const Value& listGateBlueR = jSon["GateBlueR"];
		
			for (SizeType i = 0; i < listGateBlueR.Size(); i++)
			{
				GateBlue *gateBlueR = new GateBlue(this->textureManager, this->graphics);

				id = listGateBlueR[i]["id"].GetInt();
				x = listGateBlueR[i]["x"].GetFloat();
				y = listGateBlueR[i]["y"].GetFloat();
				gateBlueR->setPosition(VECTOR2(x, y));

				/*writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateBlueR->getSprite()->getWidth();
				bound.bottom = bound.top - gateBlueR->getSprite()->getHeight();
				gateBlueR->setBoundCollision(bound);

				gateBlueR->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, gateBlueR));
			}
		
		//writer.EndArray();

		// create GateBlue L, 25 
		/*writer.Key("GateBuleL");
		writer.StartArray();*/
		const Value& listGateBlueL = jSon["GateBuleL"];
		
			for (SizeType i = 0; i < listGateBlueL.Size(); i++)
			{
				GateBlue *gateBlueL = new GateBlue(this->textureManager, this->graphics);
				gateBlueL->setFlipX(true);

				id = listGateBlueL[i]["id"].GetInt();
				x = listGateBlueL[i]["x"].GetFloat();
				y = listGateBlueL[i]["y"].GetFloat();
				gateBlueL->setPosition(VECTOR2(x, y));

				/*		writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateBlueL->getSprite()->getWidth();
				bound.bottom = bound.top - gateBlueL->getSprite()->getHeight();
				gateBlueL->setBoundCollision(bound);

				gateBlueL->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, gateBlueL));
			}
		
		//writer.EndArray();

		// create GateRed R, 4
		/*	writer.Key("GateRedR");
		writer.StartArray();*/
		const Value& listGateRedR = jSon["GateRedR"];
		
			for (SizeType i = 0; i < listGateRedR.Size(); i++)
			{
				GateRed *gateRedR = new GateRed(this->textureManager, this->graphics);

				id = listGateRedR[i]["id"].GetInt();
				x = listGateRedR[i]["x"].GetFloat();
				y = listGateRedR[i]["y"].GetFloat();
				gateRedR->setPosition(VECTOR2(x, y));

				/*	writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateRedR->getSprite()->getWidth();
				bound.bottom = bound.top - gateRedR->getSprite()->getHeight();
				gateRedR->setBoundCollision(bound);

				gateRedR->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, gateRedR));
			}
		
		//writer.EndArray();

		// create GateRed L, 4
		/*	writer.Key("GateRedL");
		writer.StartArray();*/
		const Value& listGateRedL = jSon["GateRedL"];
		
			for (SizeType i = 0; i < listGateRedL.Size(); i++)
			{
				GateRed *gateRedL = new GateRed(this->textureManager, this->graphics);
				gateRedL->setFlipX(true);

				id = listGateRedL[i]["id"].GetInt();
				x = listGateRedL[i]["x"].GetFloat();
				y = listGateRedL[i]["y"].GetFloat();
				gateRedL->setPosition(VECTOR2(x, y));

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateRedL->getSprite()->getWidth();
				bound.bottom = bound.top - gateRedL->getSprite()->getHeight();
				gateRedL->setBoundCollision(bound);

				gateRedL->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, gateRedL));
			}
		
		//writer.EndArray();


		// Load Port , 29
		//writer.Key("Port");
		//writer.StartArray();
		const Value& listPort = jSon["Port"];
		

			for (SizeType i = 0; i < listPort.Size(); i++)
			{
				Port *port = new Port();

				id = listPort[i]["id"].GetInt();
				x = listPort[i]["x"].GetFloat();
				y = listPort[i]["y"].GetFloat();
				height = listPort[i]["height"].GetFloat();
				width = listPort[i]["width"].GetFloat();

				/*		writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.Key("height");
				writer.Double(height);
				writer.Key("width");
				writer.Double(width);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + width;
				bound.bottom = bound.top - height;
				port->setBoundCollision(bound);

				port->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, port));
			}
		
		//writer.EndArray();


#pragma endregion

#pragma region Items
		// create maru mari
		const Value& maruMari = jSon["MaruMari"];
		
			MaruMari *mm = new MaruMari(this->textureManager, this->graphics);

			id = maruMari["id"].GetInt();
			x = maruMari["x"].GetFloat();
			y = maruMari["y"].GetFloat();
			mm->setPosition(VECTOR2(x + 2, y));

			/*	writer.Key("MaruMari");
			writer.StartObject();
			writer.Key("x");
			writer.Double(x);
			writer.Key("y");
			writer.Double(y);
			writer.EndObject();*/

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + mm->getSprite()->getWidth();
			bound.bottom = bound.top - mm->getSprite()->getHeight();
			mm->setBoundCollision(bound);

			mm->setActiveBound(bound);

			map_object.insert(std::pair<int, BaseObject*>(id, mm));
		

		// create ice beam
		/*writer.Key("IceBeam");
		writer.StartArray();*/
		const Value& iceBeam = jSon["IceBeam"];
		
			for (SizeType i = 0; i < iceBeam.Size(); i++)
			{
				IceBeam *ib = new IceBeam(this->textureManager, this->graphics);

				id = iceBeam[i]["id"].GetInt();
				x = iceBeam[i]["x"].GetFloat();
				y = iceBeam[i]["y"].GetFloat();
				ib->setPosition(VECTOR2(x, y));


				/*	writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + ib->getSprite()->getWidth();
				bound.bottom = bound.top - ib->getSprite()->getHeight();
				ib->setBoundCollision(bound);

				ib->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, ib));
			}
		
		//writer.EndArray();

		// create Bomb
		/*writer.Key("Bomb");
		writer.StartArray();*/
		const Value& bomb = jSon["Bomb"];
		
			for (SizeType i = 0; i < bomb.Size(); i++)
			{
				Bomb *bm = new Bomb(this->textureManager, this->graphics);

				id = bomb[i]["id"].GetInt();
				x = bomb[i]["x"].GetFloat();
				y = bomb[i]["y"].GetFloat();
				bm->setPosition(VECTOR2(x, y));

				/*	writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/


				bound.left = x;
				bound.top = y;
				bound.right = bound.left + bm->getSprite()->getWidth();
				bound.bottom = bound.top - bm->getSprite()->getHeight();
				bm->setBoundCollision(bound);

				bm->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, bm));
			}
		
		//writer.EndArray();

		// create EnergyTank, 3
		/*writer.Key("EnergyTank");
		writer.StartArray();*/
		const Value& listEnergyTank = jSon["EnergyTank"];
	
			for (SizeType i = 0; i < listEnergyTank.Size(); i++)
			{

				EnergyTank *energyT = new EnergyTank(this->textureManager, this->graphics);

				id = listEnergyTank[i]["id"].GetInt();
				x = listEnergyTank[i]["x"].GetFloat();
				y = listEnergyTank[i]["y"].GetFloat();
				energyT->setPosition(VECTOR2(x, y));

				////writer.StartObject();
				////writer.Key("x");
				////writer.Double(x);
				////writer.Key("y");
				////writer.Double(y);
				////writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + energyT->getSprite()->getWidth();
				bound.bottom = bound.top - energyT->getSprite()->getHeight();
				energyT->setBoundCollision(bound);

				energyT->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, energyT));
			}
		
		//writer.EndArray();

		// create Long Beam
		/*	writer.Key("LongBeam");
		writer.StartArray();*/
		const Value& longBeam = jSon["LongBeam"];
		
			for (SizeType i = 0; i < longBeam.Size(); i++)
			{
				LongBeam *lb = new LongBeam(this->textureManager, this->graphics);

				id = longBeam[i]["id"].GetInt();
				x = longBeam[i]["x"].GetFloat();
				y = longBeam[i]["y"].GetFloat();
				lb->setPosition(VECTOR2(x, y));

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();


				bound.left = x;
				bound.top = y;
				bound.right = bound.left + lb->getSprite()->getWidth();
				bound.bottom = bound.top - lb->getSprite()->getHeight();
				lb->setBoundCollision(bound);

				lb->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, lb));
			}
		
		//writer.EndArray();

		// create MissileRocket
		//writer.Key("MissileRocket");
		//writer.StartArray();

		const Value& listRocket = jSon["MissileRocket"];
		
			for (SizeType i = 0; i < listRocket.Size(); i++)
			{
				MissileRocket *rocket = new MissileRocket(this->textureManager, this->graphics);

				id = listRocket[i]["id"].GetInt();
				x = listRocket[i]["x"].GetFloat();
				y = listRocket[i]["y"].GetFloat();
				rocket->setPosition(VECTOR2(x, y));

				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/


				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rocket->getSprite()->getWidth();
				bound.bottom = bound.top - rocket->getSprite()->getHeight();
				rocket->setBoundCollision(bound);

				rocket->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, rocket));
			}
		
		//writer.EndArray();

		// create Varia
		/*	writer.Key("Varia");
		writer.StartArray();*/

		const Value& varia = jSon["Varia"];
		
			for (SizeType i = 0; i < varia.Size(); i++)
			{
				Varia *va = new Varia(this->textureManager, this->graphics);

				id = varia[i]["id"].GetInt();
				x = varia[i]["x"].GetFloat();
				y = varia[i]["y"].GetFloat();
				va->setPosition(VECTOR2(x, y));

				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/


				bound.left = x;
				bound.top = y;
				bound.right = bound.left + va->getSprite()->getWidth();
				bound.bottom = bound.top - va->getSprite()->getHeight();
				va->setBoundCollision(bound);

				va->setActiveBound(bound);


				map_object.insert(std::pair<int, BaseObject*>(id, va));
			}
		
		//writer.EndArray();

#pragma endregion

#pragma region Enemies
		// create AlienBig
		const Value& alienBig = jSon["AlienBig"];
	
			AlienBig *alienB = new AlienBig(this->textureManager, this->graphics);

			id = alienBig["id"].GetInt();
			x = alienBig["x"].GetFloat();
			y = alienBig["y"].GetFloat();

			x += alienB->getSprite()->getWidth()*0.5f;
			y -= alienB->getSprite()->getHeight()*0.5f;
			alienB->setPosition(VECTOR2(x, y));

			const Value& arrayWall = alienBig["ListBrickID"];
			for (SizeType t = 0; t < arrayWall.Size(); t++)
			{
				alienB->getListBrick()->push_back(static_cast<Brick*>(map_object.find(arrayWall[t].GetInt())->second));
			}
			//writer.Key("AlienBig");
			//writer.StartObject();
			//writer.Key("x");
			//writer.Double(x);
			//writer.Key("y");
			//writer.Double(y);
			//writer.EndObject();


			alienB->setBoundCollision();

			alienB->setActiveBound(alienB->getBoundCollision());


			map_object.insert(std::pair<int, BaseObject*>(id, alienB));
		

		// create AlienSmall
		const Value& alienSmall = jSon["AlienSmall"];
		

			AlienBig* big = NULL;
			big = static_cast<AlienBig*>((map_object.find(alienSmall["AlienBigID"].GetInt())->second));

			AlienSmall *alienS = new AlienSmall(this->textureManager, this->graphics, big);
			id = alienSmall["id"].GetInt();
			x = alienSmall["x"].GetFloat();
			y = alienSmall["y"].GetFloat();
			x += alienS->getSprite()->getWidth()*0.5f;
			y -= alienS->getSprite()->getHeight()*0.5f;
			alienS->setPosition(VECTOR2(x, y));

			//writer.Key("AlienSmall");
			//writer.StartObject();
			//writer.Key("x");
			//writer.Double(x);
			//writer.Key("y");
			//writer.Double(y);
			//writer.EndObject();

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + alienS->getSprite()->getWidth();
			bound.bottom = bound.top - alienS->getSprite()->getHeight();

			alienS->setBoundCollision();
			alienS->setActiveBound(alienS->getBoundCollision());


			map_object.insert(std::pair<int, BaseObject*>(id, alienS));
		

		// create ZommerYellow
		/*	writer.Key("ZommerYellow");
		writer.StartArray();*/
		const Value& listZommerYellow = jSon["ZommerYellow"];

			for (SizeType i = 0; i < listZommerYellow.Size(); i++)
			{
				Zommer *zmy = new Zommer(this->textureManager, this->graphics, EnemyColors::Yellow);

				id = listZommerYellow[i]["id"].GetInt();
				x = listZommerYellow[i]["x"].GetFloat();
				y = listZommerYellow[i]["y"].GetFloat();

				/*	x += zmy->getSprite()->getWidth()*0.5f;
				y = y - 16 + zmy->getSprite()->getHeight()*0.5f;*/

				zmy->setPosition(VECTOR2(x, y));

				zmy->setStartPosition(VECTOR2(x, y));

				const Value& arrayWall = listZommerYellow[i]["ListCollideID"];
				for (SizeType t = 0; t < arrayWall.Size(); t++)
				{
					zmy->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
				}

				zmy->setBoundCollision();
				zmy->setStartBound(zmy->getBoundCollision());

				bound.bottom = listZommerYellow[i]["ba"].GetFloat();
				bound.top = listZommerYellow[i]["ta"].GetFloat();
				bound.left = listZommerYellow[i]["la"].GetFloat();
				bound.right = listZommerYellow[i]["ra"].GetFloat();
				zmy->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("ba");
				//writer.Double(bound.bottom);
				//writer.Key("ta");
				//writer.Double(bound.top);
				//writer.Key("la");
				//writer.Double(bound.left);
				//writer.Key("ra");
				//writer.Double(bound.right);
				//writer.EndObject();



				map_object.insert(std::pair<int, BaseObject*>(id, zmy));
			}
		
		//writer.EndArray();

		// create ZommerBrown
		/*	writer.Key("ZommerBrown");
		writer.StartArray();*/
		const Value& listZommerBrown = jSon["ZommerBrown"];
	
			for (SizeType i = 0; i < listZommerBrown.Size(); i++)
			{
					Zommer *zmb = new Zommer(this->textureManager, this->graphics, EnemyColors::Brown);

					id = listZommerBrown[i]["id"].GetInt();
					x = listZommerBrown[i]["x"].GetFloat();
					y = listZommerBrown[i]["y"].GetFloat();

					//x += zmb->getSprite()->getWidth()*0.5f;
					//y = y +1;

					zmb->setPosition(VECTOR2(x, y));
					zmb->setStartPosition(VECTOR2(x, y));
					const Value& arrayWall = listZommerBrown[i]["ListCollideID"];
					for (SizeType t = 0; t < arrayWall.Size(); t++)
					{
						zmb->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
					}
					zmb->setBoundCollision();
					zmb->setStartBound(zmb->getBoundCollision());
					if (id == 912)
					{
						int test = 0;
					}
					bound.bottom = listZommerBrown[i]["ba"].GetFloat();
					string temp = std::to_string(id);
					const char *pchar = temp.c_str();
					GAMELOG(pchar);
					bound.top = listZommerBrown[i]["ta"].GetFloat();
					bound.left = listZommerBrown[i]["la"].GetFloat();
					bound.right = listZommerBrown[i]["ra"].GetFloat();
					zmb->setActiveBound(bound);

					/*		writer.StartObject();
					writer.Key("x");
					writer.Double(x);
					writer.Key("y");
					writer.Double(y);
					writer.Key("ba");
					writer.Double(bound.bottom);
					writer.Key("ta");
					writer.Double(bound.top);
					writer.Key("la");
					writer.Double(bound.left);
					writer.Key("ra");
					writer.Double(bound.right);
					writer.EndObject();*/


					map_object.insert(std::pair<int, BaseObject*>(id, zmb));
				
			}
		
		//writer.EndArray();

		// create ZommerRed
		/*	writer.Key("ZommerRed");
		writer.StartArray();*/
		const Value& listZommerRed = jSon["ZommerRed"];
	
			for (SizeType i = 0; i < listZommerRed.Size(); i++)
			{
			
					Zommer *zmr = new Zommer(this->textureManager, this->graphics, EnemyColors::Red);

					id = listZommerRed[i]["id"].GetInt();
					x = listZommerRed[i]["x"].GetFloat();
					y = listZommerRed[i]["y"].GetFloat();


					//x += zmr->getSprite()->getWidth()*0.5f;
					//y =y - 16 + zmr->getSprite()->getHeight()*0.5f;*
					y = y + 1;
					zmr->setPosition(VECTOR2(x, y));
					zmr->setStartPosition(VECTOR2(x, y));
					const Value& arrayWall = listZommerRed[i]["ListCollideID"];

					for (SizeType t = 0; t < arrayWall.Size(); t++)
					{
						zmr->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
					}
					string temp = std::to_string(id);
					const char *pchar = temp.c_str();
					GAMELOG(pchar);
					zmr->setBoundCollision();

					zmr->setStartBound(zmr->getBoundCollision());
	
						bound.bottom = listZommerRed[i]["ba"].GetFloat();
						bound.top = listZommerRed[i]["ta"].GetFloat();
						bound.left = listZommerRed[i]["la"].GetFloat();
						bound.right = listZommerRed[i]["ra"].GetFloat();
						zmr->setActiveBound(bound);
				

				

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("ba");
				//writer.Double(bound.bottom);
				//writer.Key("ta");
				//writer.Double(bound.top);
				//writer.Key("la");
				//writer.Double(bound.left);
				//writer.Key("ra");
				//writer.Double(bound.right);
				//writer.EndObject();



					map_object.insert(std::pair<int, BaseObject*>(id, zmr));
				
			}
		
		//writer.EndArray();

		// create ZebYellow
		/*writer.Key("SpawnZebYellow");
		writer.StartArray();*/
		const Value& listZebYellow = jSon["SpawnZebYellow"];
	
			for (SizeType i = 0; i < listZebYellow.Size(); i++)
			{
			
					Zeb *zby = new Zeb(this->textureManager, this->graphics, EnemyColors::Yellow, samus);

					id = listZebYellow[i]["id"].GetInt();
					x = listZebYellow[i]["x"].GetFloat();
					y = listZebYellow[i]["y"].GetFloat();

					y = y - 16 + zby->getSprite()->getHeight()*0.5f;
					x += 8;

					zby->setStartPosition(VECTOR2(x, y));
					zby->setPosition(VECTOR2(x, y));

					zby->setBoundCollision();
					string temp = std::to_string(id);
					const char *pchar = temp.c_str();
					GAMELOG(pchar);
					/*bound.bottom = listZebYellow[i]["ba"].GetFloat();
					bound.top = listZebYellow[i]["ta"].GetFloat();
					bound.left = listZebYellow[i]["la"].GetFloat();
					bound.right = listZebYellow[i]["ra"].GetFloat();*/
					zby->setActiveBound(zby->getBoundCollision());

					/*	writer.StartObject();
					writer.Key("x");
					writer.Double(x);
					writer.Key("y");
					writer.Double(y);
					writer.Key("ba");
					writer.Double(bound.bottom);
					writer.Key("ta");
					writer.Double(bound.top);
					writer.Key("la");
					writer.Double(bound.left);
					writer.Key("ra");
					writer.Double(bound.right);
					writer.EndObject();*/



					map_object.insert(std::pair<int, BaseObject*>(id, zby));
				
			}
		
		//writer.EndArray();

		// create ZebBrown
		/*writer.Key("SpawnZebBrown");
		writer.StartArray();*/
		const Value& listZebBrown = jSon["SpawnZebBrown"];
	
			for (SizeType i = 0; i < listZebBrown.Size(); i++)
			{
			
					Zeb *zbb = new Zeb(this->textureManager, this->graphics, EnemyColors::Brown, samus);

					id = listZebBrown[i]["id"].GetInt();
					x = listZebBrown[i]["x"].GetFloat();
					y = listZebBrown[i]["y"].GetFloat();

					y = y - 16 + zbb->getSprite()->getHeight()*0.5f;
					x += 8;

					zbb->setStartPosition(VECTOR2(x, y));
					zbb->setPosition(VECTOR2(x, y));

					zbb->setBoundCollision();

					/*bound.bottom = listZebBrown[i]["ba"].GetFloat();
					bound.top = listZebBrown[i]["ta"].GetFloat();
					bound.left = listZebBrown[i]["la"].GetFloat();
					bound.right = listZebBrown[i]["ra"].GetFloat();*/
					zbb->setActiveBound(zbb->getBoundCollision());

					//writer.StartObject();
					//writer.Key("x");
					//writer.Double(x);
					//writer.Key("y");
					//writer.Double(y);
					//writer.Key("ba");
					//writer.Double(bound.bottom);
					//writer.Key("ta");
					//writer.Double(bound.top);
					//writer.Key("la");
					//writer.Double(bound.left);
					//writer.Key("ra");
					//writer.Double(bound.right);
					//writer.EndObject();



					map_object.insert(std::pair<int, BaseObject*>(id, zbb));
				
			}
		
		//writer.EndArray();

		// create ZebRed
		/*	writer.Key("SpawnZebRed");
		writer.StartArray();*/
		const Value& listZebRed = jSon["SpawnZebRed"];
		
			for (SizeType i = 0; i < listZebRed.Size(); i++)
			{
			
					Zeb *zbr = new Zeb(this->textureManager, this->graphics, EnemyColors::Red, samus);

					id = listZebRed[i]["id"].GetInt();
					x = listZebRed[i]["x"].GetFloat();
					y = listZebRed[i]["y"].GetFloat();

					y = y - 16 + zbr->getSprite()->getHeight()*0.5f;
					x += 8;

					zbr->setStartPosition(VECTOR2(x, y));
					zbr->setPosition(VECTOR2(x, y));

					zbr->setBoundCollision();

					/*bound.bottom = listZebRed[i]["ba"].GetFloat();
					bound.top = listZebRed[i]["ta"].GetFloat();
					bound.left = listZebRed[i]["la"].GetFloat();
					bound.right = listZebRed[i]["ra"].GetFloat();*/
					zbr->setActiveBound(zbr->getBoundCollision());


					/*			writer.StartObject();
					writer.Key("x");
					writer.Double(x);
					writer.Key("y");
					writer.Double(y);
					writer.Key("ba");
					writer.Double(bound.bottom);
					writer.Key("ta");
					writer.Double(bound.top);
					writer.Key("la");
					writer.Double(bound.left);
					writer.Key("ra");
					writer.Double(bound.right);
					writer.EndObject();*/



					map_object.insert(std::pair<int, BaseObject*>(id, zbr));
				
			}
		
		//writer.EndArray();

		// create WaverBrown
		/*writer.Key("WaverGreen");
		writer.StartArray();*/
		const Value& listWaverBrown = jSon["WaverGreen"];
	
			for (SizeType i = 0; i < listWaverBrown.Size(); i++)
			{
				Waver *wvb = new Waver(this->textureManager, this->graphics, EnemyColors::Brown);

				id = listWaverBrown[i]["id"].GetInt();
				x = listWaverBrown[i]["x"].GetFloat();
				y = listWaverBrown[i]["y"].GetFloat();

				wvb->setPosition(VECTOR2(x, y));
				wvb->setStartPosition(VECTOR2(x, y));
				bound.left = x;
				bound.top = y;
				bound.right = bound.left + wvb->getSprite()->getWidth();
				bound.bottom = bound.top - wvb->getSprite()->getHeight();
				wvb->setBoundCollision();
				wvb->setStartBound(wvb->getBoundCollision());
				const Value& arrayWall = listWaverBrown[i]["ListCollideID"];
				for (SizeType t = 0; t < arrayWall.Size(); t++)
				{
					wvb->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
				}

				bound.bottom = listWaverBrown[i]["ba"].GetFloat();
				bound.top = listWaverBrown[i]["ta"].GetFloat();
				bound.left = listWaverBrown[i]["la"].GetFloat();
				bound.right = listWaverBrown[i]["ra"].GetFloat();
				wvb->setActiveBound(bound);

				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.Key("ba");
				writer.Double(bound.bottom);
				writer.Key("ta");
				writer.Double(bound.top);
				writer.Key("la");
				writer.Double(bound.left);
				writer.Key("ra");
				writer.Double(bound.right);
				writer.EndObject();*/




				map_object.insert(std::pair<int, BaseObject*>(id, wvb));
			}
		
		//writer.EndArray();

		// create WaverRed
		/*writer.Key("WaverBlue");
		writer.StartArray();*/
		const Value& listWaverRed = jSon["WaverBlue"];
	
			for (SizeType i = 0; i < listWaverRed.Size(); i++)
			{
				Waver *wvr = new Waver(this->textureManager, this->graphics, EnemyColors::Red);

				id = listWaverRed[i]["id"].GetInt();
				x = listWaverRed[i]["x"].GetFloat();
				y = listWaverRed[i]["y"].GetFloat();
				wvr->setPosition(VECTOR2(x, y));
				wvr->setStartPosition(VECTOR2(x, y));
				bound.left = x;
				bound.top = y;
				bound.right = bound.left + wvr->getSprite()->getWidth();
				bound.bottom = bound.top - wvr->getSprite()->getHeight();
				wvr->setBoundCollision();
				wvr->setStartBound(wvr->getBoundCollision());

				const Value& arrayWall = listWaverRed[i]["ListCollideID"];
				for (SizeType t = 0; t < arrayWall.Size(); t++)
				{
					wvr->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
				}
				bound.bottom = listWaverRed[i]["ba"].GetFloat();
				bound.top = listWaverRed[i]["ta"].GetFloat();
				bound.left = listWaverRed[i]["la"].GetFloat();
				bound.right = listWaverRed[i]["ra"].GetFloat();
				wvr->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("ba");
				//writer.Double(bound.bottom);
				//writer.Key("ta");
				//writer.Double(bound.top);
				//writer.Key("la");
				//writer.Double(bound.left);
				//writer.Key("ra");
				//writer.Double(bound.right);
				//writer.EndObject();



				map_object.insert(std::pair<int, BaseObject*>(id, wvr));
			}
		
		//writer.EndArray();

		// create SkreeYellow
		/*	writer.Key("SkreeYellow");
		writer.StartArray();*/
		const Value& listSkreeYellow = jSon["SkreeYellow"];
	
			for (SizeType i = 0; i < listSkreeYellow.Size(); i++)
			{
				Skree *sky = new Skree(this->textureManager, this->graphics, EnemyColors::Yellow);

				id = listSkreeYellow[i]["id"].GetInt();
				x = listSkreeYellow[i]["x"].GetFloat();
				y = listSkreeYellow[i]["y"].GetFloat();
				sky->setInitPosition(VECTOR2(x + sky->getSprite()->getWidth()*0.5f, y));

				sky->setBoundCollision();
				//sky->setStartBound(sky->getBoundCollision());
				const Value& arrayWall = listSkreeYellow[i]["ListCollideID"];
				for (SizeType t = 0; t < arrayWall.Size(); t++)
				{
					sky->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
				}

				bound.bottom = listSkreeYellow[i]["ba"].GetFloat();
				bound.top = listSkreeYellow[i]["ta"].GetFloat();
				bound.left = listSkreeYellow[i]["la"].GetFloat();
				bound.right = listSkreeYellow[i]["ra"].GetFloat();
				sky->setActiveBound(bound);
				sky->setStartBound(sky->getBoundCollision());
				/*		writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.Key("ba");
				writer.Double(bound.bottom);
				writer.Key("ta");
				writer.Double(bound.top);
				writer.Key("la");
				writer.Double(bound.left);
				writer.Key("ra");
				writer.Double(bound.right);
				writer.EndObject();*/



				map_object.insert(std::pair<int, BaseObject*>(id, sky));
			}
		
		//writer.EndArray();

		// create SkreeBrown
		/*writer.Key("SkreeBrown");
		writer.StartArray();*/
		const Value& listSkreeBrown = jSon["SkreeBrown"];
	
			for (SizeType i = 0; i < listSkreeBrown.Size(); i++)
			{
				Skree *skb = new Skree(this->textureManager, this->graphics, EnemyColors::Brown);

				id = listSkreeBrown[i]["id"].GetInt();
				x = listSkreeBrown[i]["x"].GetFloat();
				y = listSkreeBrown[i]["y"].GetFloat();
				skb->setInitPosition(VECTOR2(x + skb->getSprite()->getWidth()*0.5f, y));

				skb->setBoundCollision();
				//skb->setStartBound(skb->getBoundCollision());
				const Value& arrayWall = listSkreeBrown[i]["ListCollideID"];
				for (SizeType t = 0; t < arrayWall.Size(); t++)
				{
					skb->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
				}

				bound.bottom = listSkreeBrown[i]["ba"].GetFloat();
				bound.top = listSkreeBrown[i]["ta"].GetFloat();
				bound.left = listSkreeBrown[i]["la"].GetFloat();
				bound.right = listSkreeBrown[i]["ra"].GetFloat();
				skb->setActiveBound(bound);
				skb->setStartBound(skb->getBoundCollision());
				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.Key("ba");
				writer.Double(bound.bottom);
				writer.Key("ta");
				writer.Double(bound.top);
				writer.Key("la");
				writer.Double(bound.left);
				writer.Key("ra");
				writer.Double(bound.right);
				writer.EndObject();*/



				map_object.insert(std::pair<int, BaseObject*>(id, skb));
			}
		
		//writer.EndArray();

		// create RipperYellow
		/*	writer.Key("RipperYellow");
		writer.StartArray();*/
		const Value& listRipperYellow = jSon["RipperYellow"];
	
			for (SizeType i = 0; i < listRipperYellow.Size(); i++)
			{
				Ripper *rpy = new Ripper(this->textureManager, this->graphics, EnemyColors::Yellow);

				id = listRipperYellow[i]["id"].GetInt();
				x = listRipperYellow[i]["x"].GetFloat();
				y = listRipperYellow[i]["y"].GetFloat();

				rpy->setPosition(VECTOR2(x, y));

				const Value& arrayWall = listRipperYellow[i]["ListCollideID"];
				for (SizeType t = 0; t < arrayWall.Size(); t++)
				{
					rpy->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
				}

				rpy->setBoundCollision();
				string temp = std::to_string(id);
				const char *pchar = temp.c_str();
				GAMELOG(pchar);
				bound.bottom = listRipperYellow[i]["ba"].GetFloat();
				bound.top = listRipperYellow[i]["ta"].GetFloat();
				bound.left = listRipperYellow[i]["la"].GetFloat();
				bound.right = listRipperYellow[i]["ra"].GetFloat();
				rpy->setActiveBound(bound);


				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.Key("ba");
				writer.Double(bound.bottom);
				writer.Key("ta");
				writer.Double(bound.top);
				writer.Key("la");
				writer.Double(bound.left);
				writer.Key("ra");
				writer.Double(bound.right);
				writer.EndObject();*/



				map_object.insert(std::pair<int, BaseObject*>(id, rpy));
			}
		
		//writer.EndArray();

		// create RipperBrown
		/*	writer.Key("RipperBrown");
		writer.StartArray();*/
		const Value& listRipperBrown = jSon["RipperBrown"];
		
			for (SizeType i = 0; i < listRipperBrown.Size(); i++)
			{
			
					Ripper *rpb = new Ripper(this->textureManager, this->graphics, EnemyColors::Brown);

					id = listRipperBrown[i]["id"].GetInt();
					x = listRipperBrown[i]["x"].GetFloat();
					y = listRipperBrown[i]["y"].GetFloat();

					rpb->setPosition(VECTOR2(x, y));

					const Value& arrayWall = listRipperBrown[i]["ListCollideID"];
					for (SizeType t = 0; t < arrayWall.Size(); t++)
					{
						rpb->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
					}


					rpb->setBoundCollision();
					string temp = std::to_string(id);
					const char *pchar = temp.c_str();
					GAMELOG(pchar);
					bound.bottom = listRipperBrown[i]["ba"].GetFloat();
					bound.top = listRipperBrown[i]["ta"].GetFloat();
					bound.left = listRipperBrown[i]["la"].GetFloat();
					bound.right = listRipperBrown[i]["ra"].GetFloat();
					rpb->setActiveBound(bound);

					//writer.StartObject();
					//writer.Key("x");
					//writer.Double(x);
					//writer.Key("y");
					//writer.Double(y);
					//writer.Key("ba");
					//writer.Double(bound.bottom);
					//writer.Key("ta");
					//writer.Double(bound.top);
					//writer.Key("la");
					//writer.Double(bound.left);
					//writer.Key("ra");
					//writer.Double(bound.right);
					//writer.EndObject();



					map_object.insert(std::pair<int, BaseObject*>(id, rpb));
				
			}
		
		//writer.EndArray();

		// create RipperRed
		/*	writer.Key("RipperRed");
		writer.StartArray();*/
		const Value& listRipperRed = jSon["RipperRed"];
		
			for (SizeType i = 0; i < listRipperRed.Size(); i++)
			{
				Ripper *rpr = new Ripper(this->textureManager, this->graphics, EnemyColors::Red);

				id = listRipperRed[i]["id"].GetInt();
				x = listRipperRed[i]["x"].GetFloat();
				y = listRipperRed[i]["y"].GetFloat();

				rpr->setPosition(VECTOR2(x, y));

				const Value& arrayWall = listRipperRed[i]["ListCollideID"];
				for (SizeType t = 0; t < arrayWall.Size(); t++)
				{
					rpr->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
				}

				rpr->setBoundCollision();
				string temp = std::to_string(id);
				const char *pchar = temp.c_str();
				GAMELOG(pchar);
				bound.bottom = listRipperRed[i]["ba"].GetFloat();
				bound.top = listRipperRed[i]["ta"].GetFloat();
				bound.left = listRipperRed[i]["la"].GetFloat();
				bound.right = listRipperRed[i]["ra"].GetFloat();
				rpr->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("ba");
				//writer.Double(bound.bottom);
				//writer.Key("ta");
				//writer.Double(bound.top);
				//writer.Key("la");
				//writer.Double(bound.left);
				//writer.Key("ra");
				//writer.Double(bound.right);
				//writer.EndObject();



				map_object.insert(std::pair<int, BaseObject*>(id, rpr));
			}
		
		//writer.EndArray();

		// create RioYellow
		/*	writer.Key("RioYellow");
		writer.StartArray();*/
		const Value& listRioYellow = jSon["RioYellow"];
		
			for (SizeType i = 0; i < listRioYellow.Size(); i++)
			{
				Rio *roy = new Rio(this->textureManager, this->graphics, EnemyColors::Yellow);

				id = listRioYellow[i]["id"].GetInt();
				x = listRioYellow[i]["x"].GetFloat();
				y = listRioYellow[i]["y"].GetFloat();

				roy->reInit(VECTOR2(x, y));

				roy->setStartPosition(VECTOR2(x, y));

				roy->setBoundCollision();
				roy->setStartBound(roy->getBoundCollision());
				bound.bottom = listRioYellow[i]["ba"].GetFloat();
				bound.top = listRioYellow[i]["ta"].GetFloat();
				bound.left = listRioYellow[i]["la"].GetFloat();
				bound.right = listRioYellow[i]["ra"].GetFloat();
				roy->setActiveBound(bound);

				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.Key("ba");
				writer.Double(bound.bottom);
				writer.Key("ta");
				writer.Double(bound.top);
				writer.Key("la");
				writer.Double(bound.left);
				writer.Key("ra");
				writer.Double(bound.right);
				writer.EndObject();*/



				map_object.insert(std::pair<int, BaseObject*>(id, roy));
			}
		
		//writer.EndArray();

		// create RioBrown
		/*	writer.Key("RioBrown");
		writer.StartArray();*/
		const Value& listRioBrown = jSon["RioBrown"];
	
			for (SizeType i = 0; i < listRioBrown.Size(); i++)
			{
				
					Rio *rob = new Rio(this->textureManager, this->graphics, EnemyColors::Brown);

					id = listRioBrown[i]["id"].GetInt();
					x = listRioBrown[i]["x"].GetFloat();
					y = listRioBrown[i]["y"].GetFloat();

					rob->reInit(VECTOR2(x, y));

					rob->setBoundCollision();
					rob->setStartBound(rob->getBoundCollision());
					rob->setStartPosition(VECTOR2(x, y));

					string temp = std::to_string(id);
					const char *pchar = temp.c_str();
					GAMELOG(pchar);
					/*bound.bottom = listRioBrown[i]["ba"].GetFloat();
					bound.top = listRioBrown[i]["ta"].GetFloat();
					bound.left = listRioBrown[i]["la"].GetFloat();
					bound.right = listRioBrown[i]["ra"].GetFloat();*/
					rob->setActiveBound(rob->getBoundCollision());

					/*			writer.StartObject();
					writer.Key("x");
					writer.Double(x);
					writer.Key("y");
					writer.Double(y);
					writer.Key("ba");
					writer.Double(bound.bottom);
					writer.Key("ta");
					writer.Double(bound.top);
					writer.Key("la");
					writer.Double(bound.left);
					writer.Key("ra");
					writer.Double(bound.right);
					writer.EndObject();*/



					map_object.insert(std::pair<int, BaseObject*>(id, rob));
				
			}
		
		//writer.EndArray();

		// create RioRed
		/*	writer.Key("RioRed");
		writer.StartArray();*/
		const Value& listRioRed = jSon["RioRed"];
	
			for (SizeType i = 0; i < listRioRed.Size(); i++)
			{
				Rio *ror = new Rio(this->textureManager, this->graphics, EnemyColors::Red);

				id = listRioRed[i]["id"].GetInt();
				x = listRioRed[i]["x"].GetFloat();
				y = listRioRed[i]["y"].GetFloat();

				y = y - 16 + ror->getSprite()->getHeight()*0.5f;
				x = x + ror->getSprite()->getWidth()*0.5f;
				ror->setPosition(VECTOR2(x, y));
				ror->setStartPosition(VECTOR2(x, y));
				ror->setBoundCollision();
				ror->setStartBound(ror->getBoundCollision());
				/*bound.bottom = listRioRed[i]["ba"].GetFloat();
				bound.top = listRioRed[i]["ta"].GetFloat();
				bound.left = listRioRed[i]["la"].GetFloat();
				bound.right = listRioRed[i]["ra"].GetFloat();*/
				ror->setActiveBound(ror->getBoundCollision());

				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.Key("ba");
				writer.Double(bound.bottom);
				writer.Key("ta");
				writer.Double(bound.top);
				writer.Key("la");
				writer.Double(bound.left);
				writer.Key("ra");
				writer.Double(bound.right);
				writer.EndObject();*/



				map_object.insert(std::pair<int, BaseObject*>(id, ror));
			}
		
		//	writer.EndArray();




#pragma endregion

#pragma region Boss
		// create Kraid, 2
		/*writer.Key("BossKraid");
		writer.StartArray();*/
		const Value& listKraid = jSon["BossKraid"];
		
			for (SizeType i = 0; i < listKraid.Size(); i++)
			{
				BossKraid *kraid = new BossKraid(this->textureManager, this->graphics, this->samus);

				id = listKraid[i]["id"].GetInt();
				x = listKraid[i]["x"].GetFloat();
				y = listKraid[i]["y"].GetFloat();
				x += kraid->getSprite()->getWidth()*0.5f;
				y -= kraid->getSprite()->getHeight()*0.5f;
				kraid->setPosition(VECTOR2(x, y));

				kraid->setStartPosition(VECTOR2(x, y));
				bound.bottom = listKraid[i]["ba"].GetFloat();
				bound.top = listKraid[i]["ta"].GetFloat();
				bound.left = listKraid[i]["la"].GetFloat();
				bound.right = listKraid[i]["ra"].GetFloat();

				kraid->setBoundCollision();

				const Value& arrayWall = listKraid[i]["ListCollideID"];
				for (SizeType t = 0; t < arrayWall.Size(); t++)
				{
					kraid->getListWallCanCollide()->insert(*map_object.find(arrayWall[t].GetInt()));
				}
				/*			writer.StartObject();
				writer.Key("x");
				writer.Double(x);
				writer.Key("y");
				writer.Double(y);
				writer.EndObject();*/


				kraid->setActiveBound(bound);



				map_object.insert(std::pair<int, BaseObject*>(id, kraid));
			}
		
		//writer.EndArray();

		// create Dragon
		const Value& dragon = jSon["Dragon"];
		
			BossDragon *dg = new BossDragon(this->textureManager, this->graphics);

			id = dragon["id"].GetInt();
			x = dragon["x"].GetFloat();
			y = dragon["y"].GetFloat();
			dg->setPosition(VECTOR2(x, y));

			/*		writer.Key("Dragon");
			writer.StartObject();
			writer.Key("x");
			writer.Double(x);
			writer.Key("y");
			writer.Double(y);
			writer.EndObject();*/

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + dg->getSprite()->getWidth();
			bound.bottom = bound.top - dg->getSprite()->getHeight();
			dg->setBoundCollision(bound);

			dg->setActiveBound(bound);



			map_object.insert(std::pair<int, BaseObject*>(id, dg));
		

		// create Mother Facker
		const Value& mother = jSon["MotherBrain"];
		
			MotherBrain *motherFacker = new MotherBrain(this->textureManager, this->graphics, samus);

			id = mother["id"].GetInt();
			x = mother["x"].GetFloat();
			y = mother["y"].GetFloat();
			x += motherFacker->getSprite()->getWidth()*0.5f;
			y -= motherFacker->getSprite()->getHeight()*0.5f;

			motherFacker->setPosition(VECTOR2(x, y));
			
			int brickID = mother["PortID"].GetInt();
			motherFacker->setPort(static_cast<Port*>(map_object.find(brickID)->second));
			/*		writer.Key("MotherBrain");
			writer.StartObject();
			writer.Key("x");
			writer.Double(x);
			writer.Key("y");
			writer.Double(y);
			writer.EndObject();*/

			bound.left = x - motherFacker->getSprite()->getWidth();
			bound.top = y + motherFacker->getSprite()->getHeight();
			bound.right = x + motherFacker->getSprite()->getWidth();
			bound.bottom = y - motherFacker->getSprite()->getHeight();
			motherFacker->setActiveBound(bound);

			bound.left = x + 16;
			bound.top = y - 63;
			bound.right = bound.left + 80;
			bound.bottom = bound.top - 65;
			motherFacker->setBoundCollision();



			map_object.insert(std::pair<int, BaseObject*>(id, motherFacker));
		

#pragma endregion



		//writer.EndObject();


		// Create QuadTree
		//MetroidRect rect(QUADTREE_H, 0, 0, QUADTREE_W);

		return true;
	}
	catch (...)
	{

	}
	return false;
}




ObjectManager::~ObjectManager()
{
}



