#include "ObjectManager.h"
#include "GameLog.h"
#include "MaruMari.h"
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

void ObjectManager::onCheckCollision(BaseObject * obj, float dt)
{
	list<BaseObject*>* return_objects_list = new list<BaseObject*>();

		//Get all objects that can collide with current entity
	quadtree->retrieve(return_objects_list, obj);
	for (auto x = return_objects_list->begin(); x != return_objects_list->end(); x++)
	{
		Collision::getInstance()->checkCollision(obj, (*x), dt);
	}
	obj->onCollision();
	Collision::getInstance()->clearDataReturn();
	delete return_objects_list;
}

void ObjectManager::init(TextureManager * textureM, Graphics * graphics)
{
	this->textureManager = textureM; 
	this->graphics = graphics;
}

bool ObjectManager::load_list(const char * filename)
{
	VECTOR2 transform;
	if (this->camera)
		transform = VECTOR2(GAME_WIDTH*0.5f - camera->getPosition().x, GAME_HEIGHT*0.5f - camera->getPosition().y);

	try
	{
		ifstream ifs(filename);
		IStreamWrapper isw(ifs);
		Document jSon;
		jSon.ParseStream(isw);
		float x, y, height, width;

		// Load Wall POS
		const Value& listWall = jSon["Wall"];
		if (listWall.IsArray())
		{
			
			MetroidRect bound;

			for (SizeType i = 0; i < listWall.Size(); i++)
			{
				BaseObject* object = new  BaseObject(eID::WALL);
				x = listWall[i]["x"].GetFloat();
				y = listWall[i]["y"].GetFloat();
				height = listWall[i]["height"].GetFloat();
				width = listWall[i]["width"].GetFloat();

				bound.left =  x;
				bound.top =   y + 32; // Bị lệch 32bit giữa 2 layer WALL and map
				bound.right =  bound.left + width;
				bound.bottom = bound.top + height;
				object->setBoundCollision(bound);
				
				bound.left = x - 3;
				bound.top = y - 3;
				bound.right = x + width + 3;
				bound.bottom = y + height + 3;
				object->setActiveBound(bound);
				object_list->push_back(object);
			}
		}

		// create maru mari
		const Value& maruMari = jSon["MaruMari"];
		if (maruMari.IsObject())
		{
			MaruMari* mm =new MaruMari(this->textureManager, this->graphics);

			x = maruMari["x"].GetFloat();
			y = maruMari["y"].GetFloat();
			
			mm->setPosition(VECTOR2(x, y));

			MetroidRect bound;
			bound.left = x;
			bound.top = y;
			bound.right = bound.left + mm->getSprite()->getWidth();
			bound.bottom = bound.top + mm->getSprite()->getHeight();
			mm->setBoundCollision(bound);
			bound.left = x - 3;
			bound.top = y - 3;
			bound.right = x + width + 3;
			bound.bottom = y + height + 3;
			mm->setActiveBound(bound);
			object_list->push_back(mm);
		}

		MetroidRect rect(0, QUADTREE_H, 0, QUADTREE_W);
		quadtree = Quadtree::createQuadTree(rect, this->object_list);
		return true;
	}
	catch(...)
	{
		
	}
	return false;
}

ObjectManager::ObjectManager()
{
	this->object_list = new list<BaseObject*>();
}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::release()
{
	quadtree->clear();
	for (auto i = this->object_list->begin(); i != this->object_list->end(); ++i)
	{
		delete *i;
	}
	delete this->object_list;
	delete quadtree;
	delete instance;
}

void ObjectManager::setCamera(Camera * cam)
{
	this->camera = cam;
}
