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


ObjectManager* ObjectManager::instance = nullptr;
ObjectManager * ObjectManager::getInstance()
{
	if(instance==NULL)
	{
		instance = new ObjectManager();
	}

	return instance;
}

void ObjectManager::onCheckCollision(Samus * samus, float dt)
{
	if (Camera::getInstance()->getVelocity() != VECTOR2ZERO || samus->isInStatus(eStatus::START))
	{
		return_objects_list->clear();
		return_objects_list_not_wall->clear();

		RECT r = Camera::getInstance()->getBound();
		//Get all objects that can collide with current obj
		quadtree->retrieve(return_objects_list, return_objects_list_not_wall, MetroidRect((float)r.top, (float)r.bottom, (float)r.left, (float)r.right), samus);
	}

	for (auto x = return_objects_list->begin(); x != return_objects_list->end(); x++)
	{
		Collision::getInstance()->checkCollision(samus, (*x), dt);
	}

	samus->onCollision();

	Collision::getInstance()->clearDataReturn();
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
	if (return_objects_list_not_wall)
	{
		for (list<BaseObject*>::iterator i = return_objects_list_not_wall->begin(); i != return_objects_list_not_wall->end(); ++i)
		{
			(*i)->update(dt);
		}
	}
}

void ObjectManager::draw()
{
	if (return_objects_list_not_wall)
	{
		for (list<BaseObject*>::iterator i = return_objects_list_not_wall->begin(); i != return_objects_list_not_wall->end(); ++i)
		{
			(*i)->draw();
		}
	}
}

void ObjectManager::init(TextureManager * textureM, Graphics * graphics)
{
	this->textureManager = textureM; 
	this->graphics = graphics;
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
		MetroidRect bound;

#pragma region Wall
		// Load Wall POS , 650 
		const Value& listWall = jSon["Wall"];
		if (listWall.IsArray())
		{

			for (SizeType i = 0; i < listWall.Size(); i++)
			{
				BaseObject *wall = new BaseObject(eID::WALL);

				x = listWall[i]["x"].GetFloat();
				y = listWall[i]["y"].GetFloat();
				height = listWall[i]["height"].GetFloat();
				width = listWall[i]["width"].GetFloat();

				bound.left = x;
				bound.top = y + 32; // Bị lệch 32bit giữa 2 layer WALL and map
				bound.right = bound.left + width;
				bound.bottom = bound.top + height;
				wall->setBoundCollision(bound);

				wall->setActiveBound(bound);

				object_list->push_back(wall);
			}
		}
#pragma endregion

#pragma region Special Bricks
		// BrickSecretBlue, 2
		const Value& brickSecretBlue = jSon["BrickSecretBlue"];
		if (brickSecretBlue.IsObject())
		{
			Brick *bsb = new Brick(this->textureManager, this->graphics, BrickStyle::BrickSecretBlue);

			x = brickSecretBlue["x"].GetFloat();
			y = brickSecretBlue["y"].GetFloat();
			bsb->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + bsb->getSprite()->getWidth();
			bound.bottom = bound.top + bsb->getSprite()->getHeight();
			bsb->setBoundCollision(bound);

			object_list->push_back(bsb);
		}

		// BrickSerectGreen, 5
		const Value& brickSerectGreen = jSon["BrickSerectGreen"];
		if (brickSerectGreen.IsObject())
		{
			Brick *bsg = new Brick(this->textureManager, this->graphics, BrickStyle::BrickSerectGreen);

			x = brickSerectGreen["x"].GetFloat();
			y = brickSerectGreen["y"].GetFloat();
			bsg->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + bsg->getSprite()->getWidth();
			bound.bottom = bound.top + bsg->getSprite()->getHeight();
			bsg->setBoundCollision(bound);

			object_list->push_back(bsg);
		}


		// BrickGreen, 5
		const Value& brickGreen = jSon["BrickGreen"];
		if (brickGreen.IsObject())
		{
			Brick *bg = new Brick(this->textureManager, this->graphics, BrickStyle::BrickGreen);

			x = brickGreen["x"].GetFloat();
			y = brickGreen["y"].GetFloat();
			bg->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + bg->getSprite()->getWidth();
			bound.bottom = bound.top + bg->getSprite()->getHeight();
			bg->setBoundCollision(bound);

			object_list->push_back(bg);
		}


		// BrickBlue, 28, có 8 viên visible is false, so you need to check 
		bool isVisible;
		const Value& brickBlue = jSon["BrickBlue"];
		if (brickBlue.IsObject())
		{
			Brick *bg = new Brick(this->textureManager, this->graphics, BrickStyle::BrickBlue);

			x = brickBlue["x"].GetFloat();
			y = brickBlue["y"].GetFloat();
			bg->setPosition(VECTOR2(x, y));

			isVisible = brickBlue["visible"].GetBool();
			bg->setVisible(isVisible);

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + bg->getSprite()->getWidth();
			bound.bottom = bound.top + bg->getSprite()->getHeight();
			bg->setBoundCollision(bound);

			object_list->push_back(bg);
		}

		// BrickRed, 63
		const Value& brickRed = jSon["BrickRed"];
		if (brickRed.IsObject())
		{
			Brick *br = new Brick(this->textureManager, this->graphics, BrickStyle::BrickRed);

			x = brickRed["x"].GetFloat();
			y = brickRed["y"].GetFloat();
			br->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + br->getSprite()->getWidth();
			bound.bottom = bound.top + br->getSprite()->getHeight();
			br->setBoundCollision(bound);

			object_list->push_back(br);
		}

#pragma endregion

#pragma region Gates and Ports
		// create GateBlue R, 25 
		const Value& listGateBlueR = jSon["GateBlueR"];
		if (listGateBlueR.IsArray())
		{
			for (SizeType i = 0; i < listGateBlueR.Size(); i++)
			{
				GateBlue *gateBlueR = new GateBlue(this->textureManager, this->graphics);

				x = listGateBlueR[i]["x"].GetFloat();
				y = listGateBlueR[i]["y"].GetFloat();
				gateBlueR->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateBlueR->getSprite()->getWidth();
				bound.bottom = bound.top + gateBlueR->getSprite()->getHeight();
				gateBlueR->setBoundCollision(bound);

				//gateBlueR->setActiveBound(bound);

				object_list->push_back(gateBlueR);
			}
		}

		// create GateBlue L, 25 
		const Value& listGateBlueL = jSon["GateBuleL"];
		if (listGateBlueL.IsArray())
		{
			for (SizeType i = 0; i < listGateBlueL.Size(); i++)
			{
				GateBlue *gateBlueL = new GateBlue(this->textureManager, this->graphics);
				gateBlueL->setFlipX(true);

				x = listGateBlueL[i]["x"].GetFloat();
				y = listGateBlueL[i]["y"].GetFloat();
				gateBlueL->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateBlueL->getSprite()->getWidth();
				bound.bottom = bound.top + gateBlueL->getSprite()->getHeight();
				gateBlueL->setBoundCollision(bound);

				//gateBlueL->setActiveBound(bound);

				object_list->push_back(gateBlueL);
			}
		}

		// create GateRed R, 4
		const Value& listGateRedR = jSon["GateRedR"];
		if (listGateRedR.IsArray())
		{
			for (SizeType i = 0; i < listGateRedR.Size(); i++)
			{
				GateRed *gateRedR = new GateRed(this->textureManager, this->graphics);

				x = listGateRedR[i]["x"].GetFloat();
				y = listGateRedR[i]["y"].GetFloat();
				gateRedR->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateRedR->getSprite()->getWidth();
				bound.bottom = bound.top + gateRedR->getSprite()->getHeight();
				gateRedR->setBoundCollision(bound);

				gateRedR->setActiveBound(bound);

				object_list->push_back(gateRedR);
			}
		}

		// create GateRed L, 4
		const Value& listGateRedL = jSon["GateRedL"];
		if (listGateRedL.IsArray())
		{
			for (SizeType i = 0; i < listGateRedL.Size(); i++)
			{
				GateRed *gateRedL = new GateRed(this->textureManager, this->graphics);
				gateRedL->setFlipX(true);

				x = listGateRedL[i]["x"].GetFloat();
				y = listGateRedL[i]["y"].GetFloat();
				gateRedL->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + gateRedL->getSprite()->getWidth();
				bound.bottom = bound.top + gateRedL->getSprite()->getHeight();
				gateRedL->setBoundCollision(bound);

				gateRedL->setActiveBound(bound);

				object_list->push_back(gateRedL);
			}
		}


		// Load Port , 29
		const Value& listPort = jSon["Port"];
		if (listPort.IsArray())
		{

			for (SizeType i = 0; i < listPort.Size(); i++)
			{
				Port *port = new Port();

				x = listPort[i]["x"].GetFloat();
				y = listPort[i]["y"].GetFloat();
				height = listPort[i]["height"].GetFloat();
				width = listPort[i]["width"].GetFloat();

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + width;
				bound.bottom = bound.top + height;
				port->setBoundCollision(bound);

				port->setActiveBound(bound);

				object_list->push_back(port);
			}
		}


#pragma endregion

#pragma region Items
		// create maru mari
		const Value& maruMari = jSon["MaruMari"];
		if (maruMari.IsObject())
		{
			MaruMari *mm = new MaruMari(this->textureManager, this->graphics);

			x = maruMari["x"].GetFloat();
			y = maruMari["y"].GetFloat();
			mm->setPosition(VECTOR2(x + 2, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + mm->getSprite()->getWidth();
			bound.bottom = bound.top + mm->getSprite()->getHeight();
			mm->setBoundCollision(bound);

			mm->setActiveBound(bound);

			object_list->push_back(mm);
		}

		// create ice beam
		const Value& iceBeam = jSon["IceBeam"];
		if (iceBeam.IsObject())
		{
			IceBeam *ib = new IceBeam(this->textureManager, this->graphics);

			x = iceBeam["x"].GetFloat();
			y = iceBeam["y"].GetFloat();
			ib->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + ib->getSprite()->getWidth();
			bound.bottom = bound.top + ib->getSprite()->getHeight();
			ib->setBoundCollision(bound);

			object_list->push_back(ib);
		}

		// create Bomb
		const Value& bomb = jSon["Bomb"];
		if (bomb.IsObject())
		{
			Bomb *bm = new Bomb(this->textureManager, this->graphics);

			x = bomb["x"].GetFloat();
			y = bomb["y"].GetFloat();
			bm->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + bm->getSprite()->getWidth();
			bound.bottom = bound.top + bm->getSprite()->getHeight();
			bm->setBoundCollision(bound);

			object_list->push_back(bm);
		}

		// create EnergyTank, 2
		const Value& listEnergyTank = jSon["EnergyTank"];
		if (listEnergyTank.IsArray())
		{
			for (SizeType i = 0; i < listEnergyTank.Size(); i++)
			{
				EnergyTank *energyT = new EnergyTank(this->textureManager, this->graphics);

				x = listEnergyTank[i]["x"].GetFloat();
				y = listEnergyTank[i]["y"].GetFloat();
				energyT->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + energyT->getSprite()->getWidth();
				bound.bottom = bound.top + energyT->getSprite()->getHeight();
				energyT->setBoundCollision(bound);

				object_list->push_back(energyT);
			}
		}

		// create Long Beam
		const Value& longBeam = jSon["LongBeam"];
		if (longBeam.IsObject())
		{
			LongBeam *lb = new LongBeam(this->textureManager, this->graphics);

			x = longBeam["x"].GetFloat();
			y = longBeam["y"].GetFloat();
			lb->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + lb->getSprite()->getWidth();
			bound.bottom = bound.top + lb->getSprite()->getHeight();
			lb->setBoundCollision(bound);

			object_list->push_back(lb);
		}

		// create MissileRocket
		const Value& listRocket = jSon["MissileRocket"];
		if (listRocket.IsArray())
		{
			for (SizeType i = 0; i < listRocket.Size(); i++)
			{
				MissileRocket *rocket = new MissileRocket(this->textureManager, this->graphics);

				x = listRocket[i]["x"].GetFloat();
				y = listRocket[i]["y"].GetFloat();
				rocket->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rocket->getSprite()->getWidth();
				bound.bottom = bound.top + rocket->getSprite()->getHeight();
				rocket->setBoundCollision(bound);

				object_list->push_back(rocket);
			}
		}

		// create Varia
		const Value& varia = jSon["Varia"];
		if (varia.IsObject())
		{
			Varia *va = new Varia(this->textureManager, this->graphics);

			x = varia["x"].GetFloat();
			y = varia["y"].GetFloat();
			va->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + va->getSprite()->getWidth();
			bound.bottom = bound.top + va->getSprite()->getHeight();
			va->setBoundCollision(bound);

			object_list->push_back(va);
		}

#pragma endregion

#pragma region Enemies
		// create AlienBig
		const Value& alienBig = jSon["AlienBig"];
		if (alienBig.IsObject())
		{
			AlienBig *alienB = new AlienBig(this->textureManager, this->graphics);

			x = alienBig["x"].GetFloat();
			y = alienBig["y"].GetFloat();
			alienB->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + alienB->getSprite()->getWidth();
			bound.bottom = bound.top + alienB->getSprite()->getHeight();
			alienB->setBoundCollision(bound);

			alienB->setActiveBound(bound);

			object_list->push_back(alienB);
		}

		// create AlienSmall
		const Value& alienSmall = jSon["AlienSmall"];
		if (alienSmall.IsObject())
		{
			AlienSmall *alienS = new AlienSmall(this->textureManager, this->graphics);

			x = alienSmall["x"].GetFloat();
			y = alienSmall["y"].GetFloat();
			alienS->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + alienS->getSprite()->getWidth();
			bound.bottom = bound.top + alienS->getSprite()->getHeight();
			alienS->setBoundCollision(bound);

			alienS->setActiveBound(bound);

			object_list->push_back(alienS);
		}

		// create ZommerYellow
		const Value& listZommerYellow = jSon["ZommerYellow"];
		if (listZommerYellow.IsArray())
		{
			for (SizeType i = 0; i < listZommerYellow.Size(); i++)
			{
				Zommer *zmy = new Zommer(this->textureManager, this->graphics, EnemyColors::Yellow);

				x = listZommerYellow[i]["x"].GetFloat();
				y = listZommerYellow[i]["y"].GetFloat();
				zmy->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zmy->getSprite()->getWidth();
				bound.bottom = bound.top + zmy->getSprite()->getHeight();
				zmy->setBoundCollision(bound);

				zmy->setActiveBound(bound);

				object_list->push_back(zmy);
			}
		}

		// create ZommerBrown
		const Value& listZommerBrown = jSon["ZommerBrown"];
		if (listZommerBrown.IsArray())
		{
			for (SizeType i = 0; i < listZommerBrown.Size(); i++)
			{
				Zommer *zmb = new Zommer(this->textureManager, this->graphics, EnemyColors::Brown);

				x = listZommerBrown[i]["x"].GetFloat();
				y = listZommerBrown[i]["y"].GetFloat();
				zmb->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zmb->getSprite()->getWidth();
				bound.bottom = bound.top + zmb->getSprite()->getHeight();
				zmb->setBoundCollision(bound);

				zmb->setActiveBound(bound);

				object_list->push_back(zmb);
			}
		}

		// create ZommerRed
		const Value& listZommerRed = jSon["ZommerRed"];
		if (listZommerRed.IsArray())
		{
			for (SizeType i = 0; i < listZommerRed.Size(); i++)
			{
				Zommer *zmr = new Zommer(this->textureManager, this->graphics, EnemyColors::Red);

				x = listZommerRed[i]["x"].GetFloat();
				y = listZommerRed[i]["y"].GetFloat();
				zmr->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zmr->getSprite()->getWidth();
				bound.bottom = bound.top + zmr->getSprite()->getHeight();
				zmr->setBoundCollision(bound);

				zmr->setActiveBound(bound);

				object_list->push_back(zmr);
			}
		}

		// create ZebYellow
		const Value& listZebYellow = jSon["SpawnZebYellow"];
		if (listZebYellow.IsArray())
		{
			for (SizeType i = 0; i < listZebYellow.Size(); i++)
			{
				Zeb *zby = new Zeb(this->textureManager, this->graphics, EnemyColors::Yellow);

				x = listZebYellow[i]["x"].GetFloat();
				y = listZebYellow[i]["y"].GetFloat();
				zby->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zby->getSprite()->getWidth();
				bound.bottom = bound.top + zby->getSprite()->getHeight();
				zby->setBoundCollision(bound);

				zby->setActiveBound(bound);

				object_list->push_back(zby);
			}
		}

		// create ZebBrown
		const Value& listZebBrown = jSon["SpawnZebBrown"];
		if (listZebBrown.IsArray())
		{
			for (SizeType i = 0; i < listZebBrown.Size(); i++)
			{
				Zeb *zbb = new Zeb(this->textureManager, this->graphics, EnemyColors::Brown);

				x = listZebBrown[i]["x"].GetFloat();
				y = listZebBrown[i]["y"].GetFloat();
				zbb->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zbb->getSprite()->getWidth();
				bound.bottom = bound.top + zbb->getSprite()->getHeight();
				zbb->setBoundCollision(bound);

				zbb->setActiveBound(bound);

				object_list->push_back(zbb);
			}
		}

		// create ZebRed
		const Value& listZebRed = jSon["SpawnZebRed"];
		if (listZebRed.IsArray())
		{
			for (SizeType i = 0; i < listZebRed.Size(); i++)
			{
				Zeb *zbr = new Zeb(this->textureManager, this->graphics, EnemyColors::Red);

				x = listZebRed[i]["x"].GetFloat();
				y = listZebRed[i]["y"].GetFloat();
				zbr->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + zbr->getSprite()->getWidth();
				bound.bottom = bound.top + zbr->getSprite()->getHeight();
				zbr->setBoundCollision(bound);

				object_list->push_back(zbr);
			}
		}

		// create WaverBrown
		const Value& listWaverBrown = jSon["WaverGreen"];
		if (listWaverBrown.IsArray())
		{
			for (SizeType i = 0; i < listWaverBrown.Size(); i++)
			{
				Waver *wvb = new Waver(this->textureManager, this->graphics, EnemyColors::Brown);

				x = listWaverBrown[i]["x"].GetFloat();
				y = listWaverBrown[i]["y"].GetFloat();
				wvb->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + wvb->getSprite()->getWidth();
				bound.bottom = bound.top + wvb->getSprite()->getHeight();
				wvb->setBoundCollision(bound);

				object_list->push_back(wvb);
			}
		}

		// create WaverRed
		const Value& listWaverRed = jSon["WaverBlue"];
		if (listWaverRed.IsArray())
		{
			for (SizeType i = 0; i < listWaverRed.Size(); i++)
			{
				Waver *wvr = new Waver(this->textureManager, this->graphics, EnemyColors::Red);

				x = listWaverRed[i]["x"].GetFloat();
				y = listWaverRed[i]["y"].GetFloat();
				wvr->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + wvr->getSprite()->getWidth();
				bound.bottom = bound.top + wvr->getSprite()->getHeight();
				wvr->setBoundCollision(bound);

				object_list->push_back(wvr);
			}
		}

		// create SkreeYellow
		const Value& listSkreeYellow = jSon["SkreeYellow"];
		if (listSkreeYellow.IsArray())
		{
			for (SizeType i = 0; i < listSkreeYellow.Size(); i++)
			{
				Skree *sky = new Skree(this->textureManager, this->graphics, EnemyColors::Yellow);

				x = listSkreeYellow[i]["x"].GetFloat();
				y = listSkreeYellow[i]["y"].GetFloat();
				sky->initPositions(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + sky->getSprite()->getWidth();
				bound.bottom = bound.top + sky->getSprite()->getHeight();
				sky->setBoundCollision(bound);

				sky->setActiveBound(bound);

				object_list->push_back(sky);
			}
		}

		// create SkreeBrown
		const Value& listSkreeBrown = jSon["SkreeBrown"];
		if (listSkreeBrown.IsArray())
		{
			for (SizeType i = 0; i < listSkreeBrown.Size(); i++)
			{
				Skree *skb = new Skree(this->textureManager, this->graphics, EnemyColors::Brown);

				x = listSkreeBrown[i]["x"].GetFloat();
				y = listSkreeBrown[i]["y"].GetFloat();
				skb->initPositions(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + skb->getSprite()->getWidth();
				bound.bottom = bound.top + skb->getSprite()->getHeight();
				skb->setBoundCollision(bound);

				object_list->push_back(skb);
			}
		}

		// create RipperYellow
		const Value& listRipperYellow = jSon["RipperYellow"];
		if (listRipperYellow.IsArray())
		{
			for (SizeType i = 0; i < listRipperYellow.Size(); i++)
			{
				Ripper *rpy = new Ripper(this->textureManager, this->graphics, EnemyColors::Yellow);

				x = listRipperYellow[i]["x"].GetFloat();
				y = listRipperYellow[i]["y"].GetFloat();
				rpy->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rpy->getSprite()->getWidth();
				bound.bottom = bound.top + rpy->getSprite()->getHeight();
				rpy->setBoundCollision(bound);

				object_list->push_back(rpy);
			}
		}

		// create RipperBrown
		const Value& listRipperBrown = jSon["RipperBrown"];
		if (listRipperBrown.IsArray())
		{
			for (SizeType i = 0; i < listRipperBrown.Size(); i++)
			{
				Ripper *rpb = new Ripper(this->textureManager, this->graphics, EnemyColors::Brown);

				x = listRipperBrown[i]["x"].GetFloat();
				y = listRipperBrown[i]["y"].GetFloat();
				rpb->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rpb->getSprite()->getWidth();
				bound.bottom = bound.top + rpb->getSprite()->getHeight();
				rpb->setBoundCollision(bound);

				object_list->push_back(rpb);
			}
		}

		// create RipperRed
		const Value& listRipperRed = jSon["RipperRed"];
		if (listRipperRed.IsArray())
		{
			for (SizeType i = 0; i < listRipperRed.Size(); i++)
			{
				Ripper *rpr = new Ripper(this->textureManager, this->graphics, EnemyColors::Red);

				x = listRipperRed[i]["x"].GetFloat();
				y = listRipperRed[i]["y"].GetFloat();
				rpr->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rpr->getSprite()->getWidth();
				bound.bottom = bound.top + rpr->getSprite()->getHeight();
				rpr->setBoundCollision(bound);

				object_list->push_back(rpr);
			}
		}

		// create RioYellow
		const Value& listRioYellow = jSon["RioYellow"];
		if (listRioYellow.IsArray())
		{
			for (SizeType i = 0; i < listRioYellow.Size(); i++)
			{
				Rio *roy = new Rio(this->textureManager, this->graphics, EnemyColors::Yellow);

				x = listRioYellow[i]["x"].GetFloat();
				y = listRioYellow[i]["y"].GetFloat();
				roy->initPositions(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + roy->getSprite()->getWidth();
				bound.bottom = bound.top + roy->getSprite()->getHeight();
				roy->setBoundCollision(bound);

				object_list->push_back(roy);
			}
		}

		// create RioBrown
		const Value& listRioBrown = jSon["RioBrown"];
		if (listRioBrown.IsArray())
		{
			for (SizeType i = 0; i < listRioBrown.Size(); i++)
			{
				Rio *rob = new Rio(this->textureManager, this->graphics, EnemyColors::Brown);

				x = listRioBrown[i]["x"].GetFloat();
				y = listRioBrown[i]["y"].GetFloat();
				rob->initPositions(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + rob->getSprite()->getWidth();
				bound.bottom = bound.top + rob->getSprite()->getHeight();
				rob->setBoundCollision(bound);

				object_list->push_back(rob);
			}
		}

		// create RioRed
		const Value& listRioRed = jSon["RioRed"];
		if (listRioRed.IsArray())
		{
			for (SizeType i = 0; i < listRioRed.Size(); i++)
			{
				Rio *ror = new Rio(this->textureManager, this->graphics, EnemyColors::Red);

				x = listRioRed[i]["x"].GetFloat();
				y = listRioRed[i]["y"].GetFloat();
				ror->initPositions(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + ror->getSprite()->getWidth();
				bound.bottom = bound.top + ror->getSprite()->getHeight();
				ror->setBoundCollision(bound);

				object_list->push_back(ror);
			}
		}




#pragma endregion

#pragma region Boss
		// create Kraid, 2
		const Value& listKraid = jSon["BossKraid"];
		if (listKraid.IsArray())
		{
			for (SizeType i = 0; i < listKraid.Size(); i++)
			{
				BossKraid *kraid = new BossKraid(this->textureManager, this->graphics);

				x = listKraid[i]["x"].GetFloat();
				y = listKraid[i]["y"].GetFloat();
				kraid->setPosition(VECTOR2(x, y));

				bound.left = x;
				bound.top = y;
				bound.right = bound.left + kraid->getSprite()->getWidth();
				bound.bottom = bound.top + kraid->getSprite()->getHeight();
				kraid->setBoundCollision(bound);

				object_list->push_back(kraid);
			}
		}

		// create Dragon
		const Value& dragon = jSon["Dragon"];
		if (dragon.IsObject())
		{
			BossDragon *dg = new BossDragon(this->textureManager, this->graphics);

			x = dragon["x"].GetFloat();
			y = dragon["y"].GetFloat();
			dg->setPosition(VECTOR2(x, y));

			bound.left = x;
			bound.top = y;
			bound.right = bound.left + dg->getSprite()->getWidth();
			bound.bottom = bound.top + dg->getSprite()->getHeight();
			dg->setBoundCollision(bound);

			object_list->push_back(dg);
		}

#pragma endregion


		// Create QuadTree
		MetroidRect rect(0, QUADTREE_H, 0, QUADTREE_W);
		quadtree = Quadtree::createQuadTree(rect, this->object_list);

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
	this->return_objects_list = new list<BaseObject*>();
	this->return_objects_list_not_wall = new list<BaseObject*>();

	this->totalObjectsPerFrame = 0;
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

	delete return_objects_list;
	delete return_objects_list_not_wall;

	quadtree->clear();
	delete quadtree;

	delete instance;
}
