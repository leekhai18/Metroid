#include "ObjectManager.h"
#include "GameLog.h"
#include "MaruMari.h"

ObjectManager* ObjectManager::instance = nullptr;
ObjectManager * ObjectManager::getInstance()
{
	if(instance==NULL)
	{
		instance = new ObjectManager();
	}
	return instance;
}

void ObjectManager::onCheckCollision(BaseObject * obj, float frametime)
{

	for (list<BaseObject*>::iterator i = object_list->begin(); i != object_list->end(); ++i)
	{
		obj->onCollision(*i, frametime);
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

		// Load Wall POS
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

				bound.left =  x;
				bound.top =   y + 32; // Bị lệch 32bit giữa 2 layer WALL and map
				bound.right =  bound.left + width;
				bound.bottom = bound.top + height;
				wall->setBoundCollision(bound);

				object_list->push_back(wall);
			}
		}

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

			object_list->push_back(mm);
		}



		return true;
	}
	catch(...)
	{
		
	}
	return false;
}

ObjectManager::ObjectManager()
{
	object_list = new list<BaseObject*>();
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

	delete instance;
}

void ObjectManager::setCamera(Camera * cam)
{
	this->camera = cam;
}
