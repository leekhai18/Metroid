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
#include "Camera.h"
#include "Collision.h"

#include "rapidjson-master\include\rapidjson\writer.h"
#include "rapidjson-master\include\rapidjson\ostreamwrapper.h"

#define TIME_RETRIEVE 0.2f

ObjectManager* ObjectManager::instance = nullptr;

ObjectManager * ObjectManager::getInstance()
{
	if(instance==NULL)
	{
		instance = new ObjectManager();
	}

	return instance;
}

void ObjectManager::onCheckCollision(float dt)
{
	timer += dt;
	
	if (timer > TIME_RETRIEVE)
	{
		timer = 0;

		listCanCollideSamus->clear();
		listObjectNotWallOnViewPort->clear();
		listWallCanCollideSamus->clear();

		RECT r = Camera::getInstance()->getBound();
		//Get all objects that can collide with current obj
		try
		{
			quadtree->retrieve(listCanCollideSamus, listObjectNotWallOnViewPort, listWallCanCollideSamus, MetroidRect((float)r.top, (float)r.bottom, (float)r.left, (float)r.right), samus);

		}
		catch (const std::exception&)
		{
			int test = 0;
		}
	}

	// Get listCollide
	for (auto x = listCanCollideSamus->begin(); x != listCanCollideSamus->end(); x++)
	{
		Collision::getInstance()->checkCollision(samus, *x, dt);

		for (unsigned i = 0; i < BulletPool::getInstance()->getListUsing().size(); i++)
			Collision::getInstance()->checkCollision(BulletPool::getInstance()->getListUsing().at(i), *x, dt);
	}

	// handle on listCollide
	samus->onCollision();
	for (unsigned i = 0; i < BulletPool::getInstance()->getListUsing().size(); i++)
		BulletPool::getInstance()->getListUsing().at(i)->onCollision();

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
		for (list<BaseObject*>::iterator i = listObjectNotWallOnViewPort->begin(); i != listObjectNotWallOnViewPort->end(); ++i)
		{
			(*i)->update(dt);

			/*if ((*i)->getId() == eID::SKREE)
			{
				Skree* skr = static_cast<Skree*>(*i);
				skr->setTarget(samus->getPosition());
			}*/
		}
	}
}

void ObjectManager::draw()
{
	if (listObjectNotWallOnViewPort)
	{
		for (list<BaseObject*>::iterator i = listObjectNotWallOnViewPort->begin(); i != listObjectNotWallOnViewPort->end(); ++i)
		{
			(*i)->draw();
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
		BaseObject* obj = (*map_object.find(id)).second;
		node->getObjectList().push_back(obj);
	}
	if(!nodes.IsNull())
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
		if(!nodes.IsNull())
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
		//ofstream ofs("json\\objects.json");
		//OStreamWrapper osw(ofs);
		//Writer<OStreamWrapper> writer(osw);

		//writer.StartObject();

		//writer.Key("Wall");
		//writer.StartArray();

#pragma region Wall
		// Load Wall POS , 650 
		const Value& listWall = jSon["Wall"];
		if (listWall.IsArray())
		{

			for (SizeType i = 0; i < listWall.Size(); i++)
			{
				BaseObject *wall = new BaseObject(eID::WALL);

				
				id = listWall[i]["id"].GetInt();
				x = listWall[i]["x"].GetFloat();
				y = listWall[i]["y"].GetFloat(); // Bị lệch 32bit giữa 2 layer WALL and map
				height = listWall[i]["height"].GetFloat();
				width = listWall[i]["width"].GetFloat();

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
				wall->setBoundCollision(bound);

				wall->setActiveBound(bound);

				object_list->push_back(wall);

				map_object.insert(std::pair<int, BaseObject*>(id,wall));
			}
		}

		//writer.EndArray();
#pragma endregion


#pragma region Special Bricks
		// BrickSecretBlue, 2
		//writer.Key("BrickSecretBlue");
		//writer.StartArray();
		const Value& listBrickSecretBlue = jSon["BrickSecretBlue"];
		if (listBrickSecretBlue.IsArray())
		{
			for (SizeType i = 0; i < listBrickSecretBlue.Size(); i++)
			{
				Brick *bsb = new Brick(this->textureManager, this->graphics, BrickStyle::BrickSecretBlue);

				id = listBrickSecretBlue[i]["id"].GetInt();
				x = listBrickSecretBlue[i]["x"].GetFloat();
				y = listBrickSecretBlue[i]["y"].GetFloat();

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();

				bsb->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + bsb->getSprite()->getWidth();
				bound.bottom = bound.top - bsb->getSprite()->getHeight();
				bsb->setBoundCollision(bound);

				bsb->setActiveBound(bound);

				object_list->push_back(bsb);

				map_object.insert(std::pair<int, BaseObject*>(id, bsb));
			}
		}
		//writer.EndArray();

		// BrickSerectGreen, 9
		//writer.Key("BrickSerectGreen");
		//writer.StartArray();
		const Value& listBrickSerectGreen = jSon["BrickSerectGreen"];
		if (listBrickSerectGreen.IsArray())
		{
			for (SizeType i = 0; i < listBrickSerectGreen.Size(); i++)
			{
				Brick *bsg = new Brick(this->textureManager, this->graphics, BrickStyle::BrickSerectGreen);

				id = listBrickSerectGreen[i]["id"].GetInt();
				x = listBrickSerectGreen[i]["x"].GetFloat();
				y = listBrickSerectGreen[i]["y"].GetFloat();

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();

				bsg->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + bsg->getSprite()->getWidth();
				bound.bottom = bound.top - bsg->getSprite()->getHeight();
				bsg->setBoundCollision(bound);

				bsg->setActiveBound(bound);

				object_list->push_back(bsg);

				map_object.insert(std::pair<int, BaseObject*>(id, bsg));
			}
		}
		//writer.EndArray();


		// BrickGreen, 5
		//writer.Key("BrickGreen");
		//writer.StartArray();
		const Value& listBrickGreen = jSon["BrickGreen"];
		if (listBrickGreen.IsArray())
		{
			for (SizeType i = 0; i < listBrickGreen.Size(); i++)
			{
				Brick *bg = new Brick(this->textureManager, this->graphics, BrickStyle::BrickGreen);
				id = listBrickGreen[i]["id"].GetInt();

				x = listBrickGreen[i]["x"].GetFloat();
				y = listBrickGreen[i]["y"].GetFloat();
				bg->setPosition(VECTOR2(x, y));

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + bg->getSprite()->getWidth();
				bound.bottom = bound.top - bg->getSprite()->getHeight();
				bg->setBoundCollision(bound);

				bg->setActiveBound(bound);

				object_list->push_back(bg);

				map_object.insert(std::pair<int, BaseObject*>(id, bg));
			}
		}
		//writer.EndArray();


		// BrickBlue, 28, có 8 viên visible is false, so you need to check 
		//writer.Key("BrickBlue");
		//writer.StartArray();

		bool isVisible;
		const Value& listBrickBlue = jSon["BrickBlue"];
		if (listBrickBlue.IsArray())
		{
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

				object_list->push_back(bb);
				map_object.insert(std::pair<int, BaseObject*>(id, bb));
			}
		}
		//writer.EndArray();

		// BrickRed, 63
		//writer.Key("BrickRed");
		//writer.StartArray();
		const Value& listBrickRed = jSon["BrickRed"];
		if (listBrickRed.IsArray())
		{
			for (SizeType i = 0; i < listBrickRed.Size(); i++)
			{
				Brick *br = new Brick(this->textureManager, this->graphics, BrickStyle::BrickRed);

				id = listBrickRed[i]["id"].GetInt();
				x = listBrickRed[i]["x"].GetFloat();
				y = listBrickRed[i]["y"].GetFloat();
				br->setPosition(VECTOR2(x, y));

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + br->getSprite()->getWidth();
				bound.bottom = bound.top - br->getSprite()->getHeight();
				br->setBoundCollision(bound);

				br->setActiveBound(bound);

				object_list->push_back(br);
				map_object.insert(std::pair<int, BaseObject*>(id, br));
			}
		}
		//writer.EndArray();

#pragma endregion

#pragma region Gates and Ports
		// create GateBlue R, 25 
		//writer.Key("GateBlueR");
		//writer.StartArray();
		const Value& listGateBlueR = jSon["GateBlueR"];
		if (listGateBlueR.IsArray())
		{
			for (SizeType i = 0; i < listGateBlueR.Size(); i++)
			{
				GateBlue *gateBlueR = new GateBlue(this->textureManager, this->graphics);

				id = listGateBlueR[i]["id"].GetInt();
				x = listGateBlueR[i]["x"].GetFloat();
				y = listGateBlueR[i]["y"].GetFloat();
				gateBlueR->setPosition(VECTOR2(x, y));

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateBlueR->getSprite()->getWidth();
				bound.bottom = bound.top - gateBlueR->getSprite()->getHeight();
				gateBlueR->setBoundCollision(bound);

				gateBlueR->setActiveBound(bound);

				object_list->push_back(gateBlueR);
				map_object.insert(std::pair<int, BaseObject*>(id, gateBlueR));
			}
		}
		//writer.EndArray();

		// create GateBlue L, 25 
		//writer.Key("GateBuleL");
		//writer.StartArray();
		const Value& listGateBlueL = jSon["GateBuleL"];
		if (listGateBlueL.IsArray())
		{
			for (SizeType i = 0; i < listGateBlueL.Size(); i++)
			{
				GateBlue *gateBlueL = new GateBlue(this->textureManager, this->graphics);
				gateBlueL->setFlipX(true);

				id = listGateBlueL[i]["id"].GetInt();
				x = listGateBlueL[i]["x"].GetFloat();
				y = listGateBlueL[i]["y"].GetFloat();
				gateBlueL->setPosition(VECTOR2(x, y));

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateBlueL->getSprite()->getWidth();
				bound.bottom = bound.top - gateBlueL->getSprite()->getHeight();
				gateBlueL->setBoundCollision(bound);

				gateBlueL->setActiveBound(bound);

				object_list->push_back(gateBlueL);
				map_object.insert(std::pair<int, BaseObject*>(id, gateBlueL));
			}
		}
		//writer.EndArray();

		// create GateRed R, 4
		//writer.Key("GateRedR");
		//writer.StartArray();
		const Value& listGateRedR = jSon["GateRedR"];
		if (listGateRedR.IsArray())
		{
			for (SizeType i = 0; i < listGateRedR.Size(); i++)
			{
				GateRed *gateRedR = new GateRed(this->textureManager, this->graphics);

				id = listGateRedR[i]["id"].GetInt();
				x = listGateRedR[i]["x"].GetFloat();
				y = listGateRedR[i]["y"].GetFloat();
				gateRedR->setPosition(VECTOR2(x, y));

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateRedR->getSprite()->getWidth();
				bound.bottom = bound.top - gateRedR->getSprite()->getHeight();
				gateRedR->setBoundCollision(bound);

				gateRedR->setActiveBound(bound);

				object_list->push_back(gateRedR);
				map_object.insert(std::pair<int, BaseObject*>(id, gateRedR));
			}
		}
		//writer.EndArray();

		// create GateRed L, 4
		//writer.Key("GateRedL");
		//writer.StartArray();
		const Value& listGateRedL = jSon["GateRedL"];
		if (listGateRedL.IsArray())
		{
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

				object_list->push_back(gateRedL);
				map_object.insert(std::pair<int, BaseObject*>(id, gateRedL));
			}
		}
		//writer.EndArray();


		// Load Port , 29
		//writer.Key("Port");
		//writer.StartArray();
		const Value& listPort = jSon["Port"];
		if (listPort.IsArray())
		{

			for (SizeType i = 0; i < listPort.Size(); i++)
			{
				Port *port = new Port();

				id = listPort[i]["id"].GetInt();
				x = listPort[i]["x"].GetFloat();
				y = listPort[i]["y"].GetFloat();
				height = listPort[i]["height"].GetFloat();
				width = listPort[i]["width"].GetFloat();

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
				port->setBoundCollision(bound);

				port->setActiveBound(bound);

				object_list->push_back(port);
				map_object.insert(std::pair<int, BaseObject*>(id, port));
			}
		}
		//writer.EndArray();


#pragma endregion

#pragma region Items
		// create maru mari
		const Value& maruMari = jSon["MaruMari"];
		if (maruMari.IsObject())
		{
			MaruMari *mm = new MaruMari(this->textureManager, this->graphics);

			id = maruMari["id"].GetInt();
			x = maruMari["x"].GetFloat();
			y = maruMari["y"].GetFloat();
			mm->setPosition(VECTOR2(x + 2, y));

			//writer.Key("MaruMari");
			//writer.StartObject();
			//writer.Key("x");
			//writer.Double(x);
			//writer.Key("y");
			//writer.Double(y);
			//writer.EndObject();

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + mm->getSprite()->getWidth();
			bound.bottom = bound.top - mm->getSprite()->getHeight();
			mm->setBoundCollision(bound);

			mm->setActiveBound(bound);

			object_list->push_back(mm);
			map_object.insert(std::pair<int, BaseObject*>(id, mm));
		}

		// create ice beam
		const Value& iceBeam = jSon["IceBeam"];
		if (iceBeam.IsObject())
		{
			IceBeam *ib = new IceBeam(this->textureManager, this->graphics);

			id = iceBeam["id"].GetInt();
			x = iceBeam["x"].GetFloat();
			y = iceBeam["y"].GetFloat();
			ib->setPosition(VECTOR2(x, y));

			//writer.Key("IceBeam");
			//writer.StartObject();
			//writer.Key("x");
			//writer.Double(x);
			//writer.Key("y");
			//writer.Double(y);
			//writer.EndObject();

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + ib->getSprite()->getWidth();
			bound.bottom = bound.top - ib->getSprite()->getHeight();
			ib->setBoundCollision(bound);

			ib->setActiveBound(bound);

			object_list->push_back(ib);

			map_object.insert(std::pair<int, BaseObject*>(id, ib));
		}

		// create Bomb
		const Value& bomb = jSon["Bomb"];
		if (bomb.IsObject())
		{
			Bomb *bm = new Bomb(this->textureManager, this->graphics);

			id = bomb["id"].GetInt();
			x = bomb["x"].GetFloat();
			y = bomb["y"].GetFloat();
			bm->setPosition(VECTOR2(x, y));

			//writer.Key("Bomb");
			//writer.StartObject();
			//writer.Key("x");
			//writer.Double(x);
			//writer.Key("y");
			//writer.Double(y);
			//writer.EndObject();


			bound.left = x;
			bound.top = y;
			bound.right = bound.left + bm->getSprite()->getWidth();
			bound.bottom = bound.top - bm->getSprite()->getHeight();
			bm->setBoundCollision(bound);

			bm->setActiveBound(bound);

			object_list->push_back(bm);

			map_object.insert(std::pair<int, BaseObject*>(id, bm));
		}

		// create EnergyTank, 3
		//writer.Key("EnergyTank");
		//writer.StartArray();
		const Value& listEnergyTank = jSon["EnergyTank"];
		if (listEnergyTank.IsArray())
		{
			for (SizeType i = 0; i < listEnergyTank.Size(); i++)
			{
				
				EnergyTank *energyT = new EnergyTank(this->textureManager, this->graphics);

				id = listEnergyTank[i]["id"].GetInt();
				x = listEnergyTank[i]["x"].GetFloat();
				y = listEnergyTank[i]["y"].GetFloat();
				energyT->setPosition(VECTOR2(x, y));

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + energyT->getSprite()->getWidth();
				bound.bottom = bound.top - energyT->getSprite()->getHeight();
				energyT->setBoundCollision(bound);

				energyT->setActiveBound(bound);

				object_list->push_back(energyT);

				map_object.insert(std::pair<int, BaseObject*>(id, energyT));
			}
		}
		//writer.EndArray();

		// create Long Beam
		const Value& longBeam = jSon["LongBeam"];
		if (longBeam.IsObject())
		{
			LongBeam *lb = new LongBeam(this->textureManager, this->graphics);

			id = longBeam["id"].GetInt();
			x = longBeam["x"].GetFloat();
			y = longBeam["y"].GetFloat();
			lb->setPosition(VECTOR2(x, y));

			//writer.Key("LongBeam");
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

			object_list->push_back(lb);

			map_object.insert(std::pair<int, BaseObject*>(id, lb));
		}

		// create MissileRocket
		//writer.Key("MissileRocket");
		//writer.StartArray();

		const Value& listRocket = jSon["MissileRocket"];
		if (listRocket.IsArray())
		{
			for (SizeType i = 0; i < listRocket.Size(); i++)
			{
				MissileRocket *rocket = new MissileRocket(this->textureManager, this->graphics);

				id = listRocket[i]["id"].GetInt();
				x = listRocket[i]["x"].GetFloat();
				y = listRocket[i]["y"].GetFloat();
				rocket->setPosition(VECTOR2(x, y));

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();


				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rocket->getSprite()->getWidth();
				bound.bottom = bound.top - rocket->getSprite()->getHeight();
				rocket->setBoundCollision(bound);

				rocket->setActiveBound(bound);

				object_list->push_back(rocket);

				map_object.insert(std::pair<int, BaseObject*>(id, rocket));
			}
		}
		//writer.EndArray();

		// create Varia
		const Value& varia = jSon["Varia"];
		if (varia.IsObject())
		{
			Varia *va = new Varia(this->textureManager, this->graphics);

			id = varia["id"].GetInt();
			x = varia["x"].GetFloat();
			y = varia["y"].GetFloat();
			va->setPosition(VECTOR2(x, y));

			//writer.Key("Varia");
			//writer.StartObject();
			//writer.Key("x");
			//writer.Double(x);
			//writer.Key("y");
			//writer.Double(y);
			//writer.EndObject();


			bound.left = x;
			bound.top = y;
			bound.right = bound.left + va->getSprite()->getWidth();
			bound.bottom = bound.top - va->getSprite()->getHeight();
			va->setBoundCollision(bound);

			va->setActiveBound(bound);

			object_list->push_back(va);

			map_object.insert(std::pair<int, BaseObject*>(id, va));
		}

#pragma endregion

#pragma region Enemies
		// create AlienBig
		const Value& alienBig = jSon["AlienBig"];
		if (alienBig.IsObject())
		{
			AlienBig *alienB = new AlienBig(this->textureManager, this->graphics);

			id = alienBig["id"].GetInt();
			x = alienBig["x"].GetFloat();
			y = alienBig["y"].GetFloat();
			alienB->setPosition(VECTOR2(x, y));

			//writer.Key("AlienBig");
			//writer.StartObject();
			//writer.Key("x");
			//writer.Double(x);
			//writer.Key("y");
			//writer.Double(y);
			//writer.EndObject();

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + alienB->getSprite()->getWidth();
			bound.bottom = bound.top - alienB->getSprite()->getHeight();
			alienB->setBoundCollision(bound);

			alienB->setActiveBound(bound);

			object_list->push_back(alienB);

			map_object.insert(std::pair<int, BaseObject*>(id, alienB));
		}

		// create AlienSmall
		const Value& alienSmall = jSon["AlienSmall"];
		if (alienSmall.IsObject())
		{
			AlienSmall *alienS = new AlienSmall(this->textureManager, this->graphics);

			id = alienSmall["id"].GetInt();
			x = alienSmall["x"].GetFloat();
			y = alienSmall["y"].GetFloat();
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
			alienS->setBoundCollision(bound);

			alienS->setActiveBound(bound);

			object_list->push_back(alienS);

			map_object.insert(std::pair<int, BaseObject*>(id, alienS));
		}

		// create ZommerYellow
		//writer.Key("ZommerYellow");
		//writer.StartArray();
		const Value& listZommerYellow = jSon["ZommerYellow"];
		if (listZommerYellow.IsArray())
		{
			for (SizeType i = 0; i < listZommerYellow.Size(); i++)
			{
				Zommer *zmy = new Zommer(this->textureManager, this->graphics, EnemyColors::Yellow);

				id = listZommerYellow[i]["id"].GetInt();
				x = listZommerYellow[i]["x"].GetFloat();
				y = listZommerYellow[i]["y"].GetFloat();
				zmy->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zmy->getSprite()->getWidth();
				bound.bottom = bound.top - zmy->getSprite()->getHeight();
				zmy->setBoundCollision(bound);

				bound.bottom = listZommerYellow[i]["bottomA"].GetFloat();
				bound.top = listZommerYellow[i]["topA"].GetFloat();
				bound.left = listZommerYellow[i]["leftA"].GetFloat();
				bound.right = listZommerYellow[i]["rightA"].GetFloat();
				zmy->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(zmy);

				map_object.insert(std::pair<int, BaseObject*>(id, zmy));
			}
		}
		//writer.EndArray();

		// create ZommerBrown
		//writer.Key("ZommerBrown");
		//writer.StartArray();
		const Value& listZommerBrown = jSon["ZommerBrown"];
		if (listZommerBrown.IsArray())
		{
			for (SizeType i = 0; i < listZommerBrown.Size(); i++)
			{
				Zommer *zmb = new Zommer(this->textureManager, this->graphics, EnemyColors::Brown);

				id = listZommerBrown[i]["id"].GetInt();
				x = listZommerBrown[i]["x"].GetFloat();
				y = listZommerBrown[i]["y"].GetFloat();
				zmb->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zmb->getSprite()->getWidth();
				bound.bottom = bound.top - zmb->getSprite()->getHeight();
				zmb->setBoundCollision(bound);

				bound.bottom = listZommerBrown[i]["bottomA"].GetFloat();
				bound.top = listZommerBrown[i]["topA"].GetFloat();
				bound.left = listZommerBrown[i]["leftA"].GetFloat();
				bound.right = listZommerBrown[i]["rightA"].GetFloat();
				zmb->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(zmb);

				map_object.insert(std::pair<int, BaseObject*>(id, zmb));
			}
		}
		//writer.EndArray();

		// create ZommerRed
		//writer.Key("ZommerRed");
		//writer.StartArray();
		const Value& listZommerRed = jSon["ZommerRed"];
		if (listZommerRed.IsArray())
		{
			for (SizeType i = 0; i < listZommerRed.Size(); i++)
			{
				Zommer *zmr = new Zommer(this->textureManager, this->graphics, EnemyColors::Red);

				id = listZommerRed[i]["id"].GetInt();
				x = listZommerRed[i]["x"].GetFloat();
				y = listZommerRed[i]["y"].GetFloat();
				zmr->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zmr->getSprite()->getWidth();
				bound.bottom = bound.top - zmr->getSprite()->getHeight();
				zmr->setBoundCollision(bound);

				bound.bottom = listZommerRed[i]["bottomA"].GetFloat();
				bound.top = listZommerRed[i]["topA"].GetFloat();
				bound.left = listZommerRed[i]["leftA"].GetFloat();
				bound.right = listZommerRed[i]["rightA"].GetFloat();
				zmr->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(zmr);

				map_object.insert(std::pair<int, BaseObject*>(id, zmr));
			}
		}
		//writer.EndArray();

		// create ZebYellow
		//writer.Key("SpawnZebYellow");
		//writer.StartArray();
		const Value& listZebYellow = jSon["SpawnZebYellow"];
		if (listZebYellow.IsArray())
		{
			for (SizeType i = 0; i < listZebYellow.Size(); i++)
			{
				Zeb *zby = new Zeb(this->textureManager, this->graphics, EnemyColors::Yellow);

				id = listZebYellow[i]["id"].GetInt();
				x = listZebYellow[i]["x"].GetFloat();
				y = listZebYellow[i]["y"].GetFloat();
				zby->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zby->getSprite()->getWidth();
				bound.bottom = bound.top - zby->getSprite()->getHeight();
				zby->setBoundCollision(bound);

				bound.bottom = listZebYellow[i]["bottomA"].GetFloat();
				bound.top = listZebYellow[i]["topA"].GetFloat();
				bound.left = listZebYellow[i]["leftA"].GetFloat();
				bound.right = listZebYellow[i]["rightA"].GetFloat();
				zby->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(zby);

				map_object.insert(std::pair<int, BaseObject*>(id, zby));
			}
		}
		//writer.EndArray();

		// create ZebBrown
		//writer.Key("SpawnZebBrown");
		//writer.StartArray();
		const Value& listZebBrown = jSon["SpawnZebBrown"];
		if (listZebBrown.IsArray())
		{
			for (SizeType i = 0; i < listZebBrown.Size(); i++)
			{
				Zeb *zbb = new Zeb(this->textureManager, this->graphics, EnemyColors::Brown);

				id = listZebBrown[i]["id"].GetInt();
				x = listZebBrown[i]["x"].GetFloat();
				y = listZebBrown[i]["y"].GetFloat();
				zbb->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zbb->getSprite()->getWidth();
				bound.bottom = bound.top - zbb->getSprite()->getHeight();
				zbb->setBoundCollision(bound);

				bound.bottom = listZebBrown[i]["bottomA"].GetFloat();
				bound.top = listZebBrown[i]["topA"].GetFloat();
				bound.left = listZebBrown[i]["leftA"].GetFloat();
				bound.right = listZebBrown[i]["rightA"].GetFloat();
				zbb->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(zbb);

				map_object.insert(std::pair<int, BaseObject*>(id, zbb));
			}
		}
		//writer.EndArray();

		// create ZebRed
		//writer.Key("SpawnZebRed");
		//writer.StartArray();
		const Value& listZebRed = jSon["SpawnZebRed"];
		if (listZebRed.IsArray())
		{
			for (SizeType i = 0; i < listZebRed.Size(); i++)
			{
				Zeb *zbr = new Zeb(this->textureManager, this->graphics, EnemyColors::Red);

				id = listZebRed[i]["id"].GetInt();
				x = listZebRed[i]["x"].GetFloat();
				y = listZebRed[i]["y"].GetFloat();
				zbr->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zbr->getSprite()->getWidth();
				bound.bottom = bound.top - zbr->getSprite()->getHeight();
				zbr->setBoundCollision(bound);

				bound.bottom = listZebRed[i]["bottomA"].GetFloat();
				bound.top = listZebRed[i]["topA"].GetFloat();
				bound.left = listZebRed[i]["leftA"].GetFloat();
				bound.right = listZebRed[i]["rightA"].GetFloat();
				zbr->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(zbr);

				map_object.insert(std::pair<int, BaseObject*>(id, zbr));
			}
		}
		//writer.EndArray();

		// create WaverBrown
		//writer.Key("WaverGreen");
		//writer.StartArray();
		const Value& listWaverBrown = jSon["WaverGreen"];
		if (listWaverBrown.IsArray())
		{
			for (SizeType i = 0; i < listWaverBrown.Size(); i++)
			{
				Waver *wvb = new Waver(this->textureManager, this->graphics, EnemyColors::Brown);

				id = listWaverBrown[i]["id"].GetInt();
				x = listWaverBrown[i]["x"].GetFloat();
				y = listWaverBrown[i]["y"].GetFloat();
				wvb->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + wvb->getSprite()->getWidth();
				bound.bottom = bound.top - wvb->getSprite()->getHeight();
				wvb->setBoundCollision(bound);

				bound.bottom = listWaverBrown[i]["bottomA"].GetFloat();
				bound.top = listWaverBrown[i]["topA"].GetFloat();
				bound.left = listWaverBrown[i]["leftA"].GetFloat();
				bound.right = listWaverBrown[i]["rightA"].GetFloat();
				wvb->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();



				object_list->push_back(wvb);

				map_object.insert(std::pair<int, BaseObject*>(id, wvb));
			}
		}
		//writer.EndArray();

		// create WaverRed
		//writer.Key("WaverBlue");
		//writer.StartArray();
		const Value& listWaverRed = jSon["WaverBlue"];
		if (listWaverRed.IsArray())
		{
			for (SizeType i = 0; i < listWaverRed.Size(); i++)
			{
				Waver *wvr = new Waver(this->textureManager, this->graphics, EnemyColors::Red);

				id = listWaverRed[i]["id"].GetInt();
				x = listWaverRed[i]["x"].GetFloat();
				y = listWaverRed[i]["y"].GetFloat();
				wvr->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + wvr->getSprite()->getWidth();
				bound.bottom = bound.top - wvr->getSprite()->getHeight();
				wvr->setBoundCollision(bound);

				bound.bottom = listWaverRed[i]["bottomA"].GetFloat();
				bound.top = listWaverRed[i]["topA"].GetFloat();
				bound.left = listWaverRed[i]["leftA"].GetFloat();
				bound.right = listWaverRed[i]["rightA"].GetFloat();
				wvr->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(wvr);

				map_object.insert(std::pair<int, BaseObject*>(id, wvr));
			}
		}
		//writer.EndArray();

		// create SkreeYellow
		//writer.Key("SkreeYellow");
		//writer.StartArray();
		const Value& listSkreeYellow = jSon["SkreeYellow"];
		if (listSkreeYellow.IsArray())
		{
			for (SizeType i = 0; i < listSkreeYellow.Size(); i++)
			{
				Skree *sky = new Skree(this->textureManager, this->graphics, EnemyColors::Yellow);

				id = listSkreeYellow[i]["id"].GetInt();
				x = listSkreeYellow[i]["x"].GetFloat();
				y = listSkreeYellow[i]["y"].GetFloat();
				sky->setPosition(VECTOR2(x + sky->getSprite()->getWidth()*0.5f, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + sky->getSprite()->getWidth();
				bound.bottom = bound.top - sky->getSprite()->getHeight();
				sky->setBoundCollision(bound);

				bound.bottom = listSkreeYellow[i]["bottomA"].GetFloat();
				bound.top = listSkreeYellow[i]["topA"].GetFloat();
				bound.left = listSkreeYellow[i]["leftA"].GetFloat();
				bound.right = listSkreeYellow[i]["rightA"].GetFloat();
				sky->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(sky);

				map_object.insert(std::pair<int, BaseObject*>(id, sky));
			}
		}
		//writer.EndArray();

		// create SkreeBrown
		//writer.Key("SkreeBrown");
		//writer.StartArray();
		const Value& listSkreeBrown = jSon["SkreeBrown"];
		if (listSkreeBrown.IsArray())
		{
			for (SizeType i = 0; i < listSkreeBrown.Size(); i++)
			{
				Skree *skb = new Skree(this->textureManager, this->graphics, EnemyColors::Brown);

				id = listSkreeBrown[i]["id"].GetInt();
				x = listSkreeBrown[i]["x"].GetFloat();
				y = listSkreeBrown[i]["y"].GetFloat();
				skb->setPosition(VECTOR2(x + skb->getSprite()->getWidth()*0.5f, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + skb->getSprite()->getWidth();
				bound.bottom = bound.top - skb->getSprite()->getHeight();
				skb->setBoundCollision(bound);

				bound.bottom = listSkreeBrown[i]["bottomA"].GetFloat();
				bound.top = listSkreeBrown[i]["topA"].GetFloat();
				bound.left = listSkreeBrown[i]["leftA"].GetFloat();
				bound.right = listSkreeBrown[i]["rightA"].GetFloat();
				skb->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(skb);

				map_object.insert(std::pair<int, BaseObject*>(id, skb));
			}
		}
		//writer.EndArray();

		// create RipperYellow
		//writer.Key("RipperYellow");
		//writer.StartArray();
		const Value& listRipperYellow = jSon["RipperYellow"];
		if (listRipperYellow.IsArray())
		{
			for (SizeType i = 0; i < listRipperYellow.Size(); i++)
			{
				Ripper *rpy = new Ripper(this->textureManager, this->graphics, EnemyColors::Yellow);

				id = listRipperYellow[i]["id"].GetInt();
				x = listRipperYellow[i]["x"].GetFloat();
				y = listRipperYellow[i]["y"].GetFloat();
				rpy->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rpy->getSprite()->getWidth();
				bound.bottom = bound.top - rpy->getSprite()->getHeight();
				rpy->setBoundCollision(bound);

				bound.bottom = listRipperYellow[i]["bottomA"].GetFloat();
				bound.top = listRipperYellow[i]["topA"].GetFloat();
				bound.left = listRipperYellow[i]["leftA"].GetFloat();
				bound.right = listRipperYellow[i]["rightA"].GetFloat();
				rpy->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(rpy);

				map_object.insert(std::pair<int, BaseObject*>(id, rpy));
			}
		}
		//writer.EndArray();

		// create RipperBrown
		//writer.Key("RipperBrown");
		//writer.StartArray();
		const Value& listRipperBrown = jSon["RipperBrown"];
		if (listRipperBrown.IsArray())
		{
			for (SizeType i = 0; i < listRipperBrown.Size(); i++)
			{
				Ripper *rpb = new Ripper(this->textureManager, this->graphics, EnemyColors::Brown);

				id = listRipperBrown[i]["id"].GetInt();
				x = listRipperBrown[i]["x"].GetFloat();
				y = listRipperBrown[i]["y"].GetFloat();
				rpb->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rpb->getSprite()->getWidth();
				bound.bottom = bound.top - rpb->getSprite()->getHeight();
				rpb->setBoundCollision(bound);

				bound.bottom = listRipperBrown[i]["bottomA"].GetFloat();
				bound.top = listRipperBrown[i]["topA"].GetFloat();
				bound.left = listRipperBrown[i]["leftA"].GetFloat();
				bound.right = listRipperBrown[i]["rightA"].GetFloat();
				rpb->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(rpb);

				map_object.insert(std::pair<int, BaseObject*>(id, rpb));
			}
		}
		//writer.EndArray();

		// create RipperRed
		//writer.Key("RipperRed");
		//writer.StartArray();
		const Value& listRipperRed = jSon["RipperRed"];
		if (listRipperRed.IsArray())
		{
			for (SizeType i = 0; i < listRipperRed.Size(); i++)
			{
				Ripper *rpr = new Ripper(this->textureManager, this->graphics, EnemyColors::Red);

				id = listRipperRed[i]["id"].GetInt();
				x = listRipperRed[i]["x"].GetFloat();
				y = listRipperRed[i]["y"].GetFloat();
				rpr->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rpr->getSprite()->getWidth();
				bound.bottom = bound.top - rpr->getSprite()->getHeight();
				rpr->setBoundCollision(bound);

				bound.bottom = listRipperRed[i]["bottomA"].GetFloat();
				bound.top = listRipperRed[i]["topA"].GetFloat();
				bound.left = listRipperRed[i]["leftA"].GetFloat();
				bound.right = listRipperRed[i]["rightA"].GetFloat();
				rpr->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(rpr);

				map_object.insert(std::pair<int, BaseObject*>(id, rpr));
			}
		}
		//writer.EndArray();

		// create RioYellow
		//writer.Key("RioYellow");
		//writer.StartArray();
		const Value& listRioYellow = jSon["RioYellow"];
		if (listRioYellow.IsArray())
		{
			for (SizeType i = 0; i < listRioYellow.Size(); i++)
			{
				Rio *roy = new Rio(this->textureManager, this->graphics, EnemyColors::Yellow);

				id = listRioYellow[i]["id"].GetInt();
				x = listRioYellow[i]["x"].GetFloat();
				y = listRioYellow[i]["y"].GetFloat();
				roy->initPositions(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + roy->getSprite()->getWidth();
				bound.bottom = bound.top - roy->getSprite()->getHeight();
				roy->setBoundCollision(bound);

				bound.bottom = listRioYellow[i]["bottomA"].GetFloat();
				bound.top = listRioYellow[i]["topA"].GetFloat();
				bound.left = listRioYellow[i]["leftA"].GetFloat();
				bound.right = listRioYellow[i]["rightA"].GetFloat();
				roy->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(roy);

				map_object.insert(std::pair<int, BaseObject*>(id, roy));
			}
		}
		//writer.EndArray();

		// create RioBrown
		//writer.Key("RioBrown");
		//writer.StartArray();
		const Value& listRioBrown = jSon["RioBrown"];
		if (listRioBrown.IsArray())
		{
			for (SizeType i = 0; i < listRioBrown.Size(); i++)
			{
				Rio *rob = new Rio(this->textureManager, this->graphics, EnemyColors::Brown);

				id = listRioBrown[i]["id"].GetInt();
				x = listRioBrown[i]["x"].GetFloat();
				y = listRioBrown[i]["y"].GetFloat();
				rob->initPositions(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rob->getSprite()->getWidth();
				bound.bottom = bound.top - rob->getSprite()->getHeight();
				rob->setBoundCollision(bound);

				bound.bottom = listRioBrown[i]["bottomA"].GetFloat();
				bound.top = listRioBrown[i]["topA"].GetFloat();
				bound.left = listRioBrown[i]["leftA"].GetFloat();
				bound.right = listRioBrown[i]["rightA"].GetFloat();
				rob->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(rob);

				map_object.insert(std::pair<int, BaseObject*>(id, rob));
			}
		}
		//writer.EndArray();

		// create RioRed
		//writer.Key("RioRed");
		//writer.StartArray();
		const Value& listRioRed = jSon["RioRed"];
		if (listRioRed.IsArray())
		{
			for (SizeType i = 0; i < listRioRed.Size(); i++)
			{
				Rio *ror = new Rio(this->textureManager, this->graphics, EnemyColors::Red);

				id = listRioRed[i]["id"].GetInt();
				x = listRioRed[i]["x"].GetFloat();
				y = listRioRed[i]["y"].GetFloat();
				ror->initPositions(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + ror->getSprite()->getWidth();
				bound.bottom = bound.top - ror->getSprite()->getHeight();
				ror->setBoundCollision(bound);

				bound.bottom = listRioRed[i]["bottomA"].GetFloat();
				bound.top = listRioRed[i]["topA"].GetFloat();
				bound.left = listRioRed[i]["leftA"].GetFloat();
				bound.right = listRioRed[i]["rightA"].GetFloat();
				ror->setActiveBound(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.Key("bottomA");
				//writer.Double(bound.bottom);
				//writer.Key("topA");
				//writer.Double(bound.top);
				//writer.Key("leftA");
				//writer.Double(bound.left);
				//writer.Key("rightA");
				//writer.Double(bound.right);
				//writer.EndObject();


				object_list->push_back(ror);

				map_object.insert(std::pair<int, BaseObject*>(id, ror));
			}
		}
		//writer.EndArray();




#pragma endregion

#pragma region Boss
		// create Kraid, 2
		//writer.Key("BossKraid");
		//writer.StartArray();
		const Value& listKraid = jSon["BossKraid"];
		if (listKraid.IsArray())
		{
			for (SizeType i = 0; i < listKraid.Size(); i++)
			{
				BossKraid *kraid = new BossKraid(this->textureManager, this->graphics);

				id = listKraid[i]["id"].GetInt();
				x = listKraid[i]["x"].GetFloat();
				y = listKraid[i]["y"].GetFloat();
				kraid->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + kraid->getSprite()->getWidth();
				bound.bottom = bound.top - kraid->getSprite()->getHeight();
				kraid->setBoundCollision(bound);

				//writer.StartObject();
				//writer.Key("x");
				//writer.Double(x);
				//writer.Key("y");
				//writer.Double(y);
				//writer.EndObject();


				kraid->setActiveBound(bound);

				object_list->push_back(kraid);
				map_object.insert(std::pair<int, BaseObject*>(id, kraid));
			}
		}
		//writer.EndArray();

		// create Dragon
		const Value& dragon = jSon["Dragon"];
		if (dragon.IsObject())
		{
			BossDragon *dg = new BossDragon(this->textureManager, this->graphics);

			id = dragon["id"].GetInt();
			x = dragon["x"].GetFloat();
			y = dragon["y"].GetFloat();
			dg->setPosition(VECTOR2(x, y));

			//writer.Key("Dragon");
			//writer.StartObject();
			//writer.Key("x");
			//writer.Double(x);
			//writer.Key("y");
			//writer.Double(y);
			//writer.EndObject();

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + dg->getSprite()->getWidth();
			bound.bottom = bound.top - dg->getSprite()->getHeight();
			dg->setBoundCollision(bound);

			dg->setActiveBound(bound);

			object_list->push_back(dg);

			map_object.insert(std::pair<int, BaseObject*>(id, dg));
		}

#pragma endregion

		//writer.EndObject();


		// Create QuadTree
		//MetroidRect rect(QUADTREE_H, 0, 0, QUADTREE_W);
		//quadtree = Quadtree::createQuadTree(rect, this->object_list);

		return true;
	}
	catch (...)
	{

	}
	return false;
}


ObjectManager::ObjectManager()
{
	this->object_list = new list<BaseObject*>();
	this->listCanCollideSamus = new list<BaseObject*>();
	this->listObjectNotWallOnViewPort = new list<BaseObject*>();
	this->listWallCanCollideSamus = new list<BaseObject*>();

	this->totalObjectsPerFrame = 0;
	this->timer = 0;
}


ObjectManager::~ObjectManager()
{
}



void ObjectManager::release()
{
	for (list<BaseObject*>::iterator i = object_list->begin(); i != object_list->end(); ++i)
	{
		delete *i;
	}
	object_list->clear();
	delete object_list;

	delete listCanCollideSamus;
	delete listObjectNotWallOnViewPort;
	delete listWallCanCollideSamus;

	quadtree->clear();
	delete quadtree;

	delete instance;
}
