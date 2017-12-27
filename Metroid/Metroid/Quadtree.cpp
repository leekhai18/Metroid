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

map<int, BaseObject*>& Quadtree::getObjectList()
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



void Quadtree::retrieve(map<int, BaseObject*>* listNotWallCanCollideSamus, map<int, BaseObject*>* listObjectNotWallOnViewPort, 
	map<int, BaseObject*>* listWallCanCollideSamus, map<int, BaseObject*>* listWallEnermy,MetroidRect rect, BaseObject* samus)
{
	if (nodes)
	{
		for (auto node = nodes->begin(); node != nodes->end(); ++node)
		{
			if ((*node)->isContain(rect))
			{
				(*node)->retrieve(listNotWallCanCollideSamus, listObjectNotWallOnViewPort, listWallCanCollideSamus, listWallEnermy,rect, samus);
			}
		}

		return; // Return here to ignore rest.
	}

	//Add all entities in current region into return_objects_list
	/*if (this->isContain(rect))
	{*/
		for (auto i = objects_list.begin(); i != objects_list.end(); i++)
		{
			
			if (((*i).second)->getId() != eID::WALL)
			{
				/*if (listNotWallCanCollideSamus->find((*i).first) == listNotWallCanCollideSamus->end())
				{*/
					if (Collision::getInstance()->isCollide(samus->getActiveBound(), ((*i).second)->getBoundCollision()))
					{
						listNotWallCanCollideSamus->insert((*i));
					}
				//}
				//if (listObjectNotWallOnViewPort->find((*i).first) == listObjectNotWallOnViewPort->end())
				//{
					if (Collision::getInstance()->isCollide(rect, ((*i).second)->getBoundCollision())) // Must bound collision, will fixed after
					{
						listObjectNotWallOnViewPort->insert(*i);
					}
				//}
					if(((*i).second)->getId()== eID::GATEBLUE || ((*i).second)->getId() == eID::GATERED)
					{
						if (Collision::getInstance()->isCollide(rect, ((*i).second)->getBoundCollision()))
						{
							listWallEnermy->insert(*i);
						}
					}
			}
			else
			{
				//if (listWallCanCollideSamus->find((*i).first) == listWallCanCollideSamus->end())
				//{
				if (Collision::getInstance()->isCollide(samus->getActiveBound(), ((*i).second)->getBoundCollision()))
				{
					listWallCanCollideSamus->insert(*i);
				}
				//}
				//if (listWallEnermy->find((*i).first) == listWallEnermy->end())
				//{
				if (Collision::getInstance()->isCollide(rect, ((*i).second)->getBoundCollision()))
				{
					listWallEnermy->insert(*i);
				}

				//}
			}
			/*if (((*i).second)->getId() == eID::WALL )
			{
				
			}*/


		}
	//}
}


