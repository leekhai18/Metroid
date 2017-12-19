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



void Quadtree::retrieve(list<BaseObject*>* listNotWallCanCollideSamus, list<BaseObject*>* listObjectNotWallOnViewPort, list<BaseObject*>* listWallCanCollideSamus, MetroidRect rect, BaseObject* samus)
{
	if (nodes)
	{
		for (auto node = nodes->begin(); node != nodes->end(); ++node)
		{
			if ((*node)->isContain(rect))
			{
				(*node)->retrieve(listNotWallCanCollideSamus, listObjectNotWallOnViewPort, listWallCanCollideSamus, rect, samus);
			}
		}

		return; // Return here to ignore rest.
	}

	//Add all entities in current region into return_objects_list
	if (this->isContain(rect))
	{
		for (auto i = objects_list.begin(); i != objects_list.end(); i++)
		{
			bool found1 = (std::find(listNotWallCanCollideSamus->begin(), listNotWallCanCollideSamus->end(), *i) != listNotWallCanCollideSamus->end());
			bool found2 = (std::find(listWallCanCollideSamus->begin(), listWallCanCollideSamus->end(), *i) != listWallCanCollideSamus->end());
			bool found3 = (std::find(listObjectNotWallOnViewPort->begin(), listObjectNotWallOnViewPort->end(), *i) != listObjectNotWallOnViewPort->end());

			if (!found1)
			{
				if (Collision::getInstance()->isCollide(samus->getActiveBound(), (*i)->getActiveBound()))
				{
					if ((*i)->getId() != eID::WALL)
					{
						listNotWallCanCollideSamus->push_back(*i);
					}
				}
			}

			if (!found2)
			{
				if (Collision::getInstance()->isCollide(samus->getActiveBound(), (*i)->getActiveBound()))
				{
					if ((*i)->getId() == eID::WALL)
						listWallCanCollideSamus->push_back(*i);
				}
			}

			if (!found3)
			{
				if (Collision::getInstance()->isCollide(rect, (*i)->getBoundCollision())) // Must bound collision, will fixed after
				{
					if ((*i)->getId() != eID::WALL)
						listObjectNotWallOnViewPort->push_back(*i);
				}
			}
		}
	}
}


