#include "Quadtree.h"
#include <algorithm>
#include "Collision.h"


bool Quadtree::isContain(MetroidRect bound)
{
	if (bound.right >= region.left&&bound.left <= region.right&&bound.bottom <= region.top&&bound.top >= region.bottom)
	{
		return true;
	}
	return false;
}

void Quadtree::split()
{
    nodes = new list<Quadtree*>();

	nodes->push_back(new Quadtree(level + 1, MetroidRect(region.top, region.top / 2, region.left, region.right / 2)));

	nodes->push_back(new Quadtree(level + 1, MetroidRect(region.top, region.top / 2, region.right / 2, region.right)));

	nodes->push_back(new Quadtree(level + 1, MetroidRect(region.top / 2, region.bottom, region.left, region.right / 2)));

	nodes->push_back(new Quadtree(level + 1, MetroidRect(region.top / 2, region.bottom, region.right / 2, region.right)));

}

Quadtree::Quadtree()
{
}

list<BaseObject*>& Quadtree::getObjectList()
{
	return this->objects_list;
}

list<Quadtree*>*& Quadtree::getNodes()
{
	return this->nodes;
}

MetroidRect Quadtree::getRegion()
{
	return this->region;
}

int Quadtree::getLevel()
{
	return this->level;
}

Quadtree::Quadtree(int level, MetroidRect region)
{
	this->level = level;
	this->region = region;
	//objects_list = new list<BaseObject*>();
}


Quadtree::~Quadtree()
{
	
}

void Quadtree::clear()
{
	//Clear all nodes
	if (nodes)
	{

		for (auto node = nodes->begin(); node != nodes->end(); ++node)
		{
			(*node)->clear();
		}
		delete nodes;
	}

	//Clear current quadtree
	objects_list.clear();
}

void Quadtree::insert(BaseObject * obj)
{
	//insert obj into corresponding nodes
	if (nodes)
	{
		for (auto node = nodes->begin(); node != nodes->end(); ++node)
		{
			if((*node)->isContain(obj->getActiveBound()))
			{
				(*node)->insert(obj);
			}
		}


		return; // Return here to ignore rest.
	}

	//Insert obj into current quadtree
	if (this->isContain(obj->getActiveBound()))
	{
		objects_list.push_back(obj);
	}

	//Split and move all objects in list into it’s corresponding nodes
	if (objects_list.size() > MAX_OBJECT_IN_REGION && level < MAX_LEVEL)
	{
		split();

		while (!objects_list.empty())
		{

			for (auto node = nodes->begin(); node != nodes->end(); ++node)
			{
				if ((*node)->isContain(objects_list.back()->getActiveBound()))
				{
					(*node)->insert(objects_list.back());
				}
			}
			objects_list.pop_back();
		}
	}
}

void Quadtree::retrieve(list<BaseObject*>* listCanCollideSamus, list<BaseObject*>* listObjectNotWallOnViewPort, list<BaseObject*>* listWallCanCollideSamus, MetroidRect rect, BaseObject* samus)
{
	if (nodes)
	{
		for (auto node = nodes->begin(); node != nodes->end(); ++node)
		{
			if ((*node)->isContain(rect))
			{
				(*node)->retrieve(listCanCollideSamus, listObjectNotWallOnViewPort, listWallCanCollideSamus, rect, samus);
			}
		}

		return; // Return here to ignore rest.
	}

	//Add all entities in current region into return_objects_list
	if (this->isContain(rect))
	{
		for (auto i = objects_list.begin(); i != objects_list.end(); i++)
		{
			bool found1 = (std::find(listCanCollideSamus->begin(), listCanCollideSamus->end(), *i) != listCanCollideSamus->end());
			bool found2 = (std::find(listWallCanCollideSamus->begin(), listWallCanCollideSamus->end(), *i) != listWallCanCollideSamus->end());
			bool found3 = (std::find(listObjectNotWallOnViewPort->begin(), listObjectNotWallOnViewPort->end(), *i) != listObjectNotWallOnViewPort->end());

			if (!found1)
			{
				MetroidRect objectActive = (*i)->getBoundCollision();
				objectActive.top = objectActive.top + 20;
				objectActive.left = objectActive.left - 20;
				objectActive.right = objectActive.right + 20;
				objectActive.bottom = objectActive.bottom - 20;
				if (Collision::getInstance()->isCollide(samus->getActiveBound(), (*i)->getBoundCollision()))
				{
					if ((*i)->getId() != eID::WALL)
					{
						listCanCollideSamus->push_back(*i);
					}
					
				}
			}

			if (!found2)
			{
				

				MetroidRect objectActive = (*i)->getBoundCollision();
				objectActive.top = objectActive.top + 4;
				objectActive.left = objectActive.left - 4;
				objectActive.right = objectActive.right + 4;
				objectActive.bottom = objectActive.bottom - 4;
				if (Collision::getInstance()->isCollide(samus->getActiveBound(), (*i)->getBoundCollision()))
				{
					if (this->level == 6&& (*i)->getBoundCollision().bottom==2976.0f)
					{
						int test = 0;
					}
					if ((*i)->getId() == eID::WALL)
						listWallCanCollideSamus->push_back(*i);
				}
			}

			if (!found3)
			{
				/*if ((*i)->getId() != eID::WALL)
				{
					if ((*i)->getPosition().x == 600)
					{
						int test = 0;
					}
				}*/
				
				if (Collision::getInstance()->isCollide(rect, (*i)->getBoundCollision()))
				{
					/*if((*i)->getBoundCollision().left== 1153.09998f)
					{
						int test = 0;
					}*/
					if ((*i)->getId() != eID::WALL)
						listObjectNotWallOnViewPort->push_back(*i);
				}
			}
		}
	}
}

Quadtree*  Quadtree::createQuadTree(MetroidRect rect, list<BaseObject*>* object_list)
{
	Quadtree* quadtree = new Quadtree(1, rect);

	for (auto i = object_list->begin(); i != object_list->end(); i++)
		quadtree->insert(*i);

	return quadtree;
}
