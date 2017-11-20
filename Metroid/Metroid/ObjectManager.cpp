#include "ObjectManager.h"
#include "GameLog.h"

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

	for (list<BaseObject>::iterator i = object_list.begin(); i != object_list.end(); ++i)
	{
		obj->onCollision(&*(i), frametime);
	}
}

bool ObjectManager::load_list(const char * filename)
{
	try
	{
		ifstream ifs(filename);
		IStreamWrapper isw(ifs);
		Document jSon;
		jSon.ParseStream(isw);
		const Value& listWall = jSon["Wall"];
		if (listWall.IsArray())
		{
			float x, y, height, width;
			for (SizeType i = 0; i < listWall.Size(); i++)
			{
				BaseObject object(eID::WALL);
				x = listWall[i]["x"].GetFloat();
				y = listWall[i]["y"].GetFloat();
				height = listWall[i]["height"].GetFloat();
				width = listWall[i]["width"].GetFloat();
				MetroidRect bound;
				bound.left = (float) x;
				bound.top = (float) y + 32; // Bị lệch 32bit giữa 2 layer WALL and map
				bound.right = (float) bound.left + width;
				bound.bottom = (float) bound.top + height;
				object.setBoundCollision(bound);

				object_list.push_back(object);
				}
		}
	/*	const Value& listPort = jSon["Port"];
		if (listPort.IsArray())
		{
			for (SizeType i = 0; i < listPort.Size(); i++)
			{
				BaseObject entity(eID::PORT);
				int x, y, height, width;
				x = listPort[i]["x"].GetInt();
				y = listPort[i]["y"].GetInt();
				height = listPort[i]["height"].GetInt();
				width = listPort[i]["width"].GetInt();
				MetroidRect bound;
				bound.left = x;
				bound.top = y;
				bound.right = x + width;
				bound.bottom = y + height;
				entity.setBoundCollision(bound);

				entity_list.push_back(entity);
			}

		}*/
		return true;
	}
	catch(...)
	{
		
	}
	return false;
}

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::release()
{
	delete instance;
}

void ObjectManager::setCamera(Camera * cam)
{
	this->camera = cam;
}
