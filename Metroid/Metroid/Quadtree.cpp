#include "Quadtree.h"
#include <algorithm>
#include "Collision.h"


bool Quadtree::isContain(MetroidRect bound)
{
	if (bound.right >= region.left&&bound.left <= region.right&&bound.bottom >= region.top&&bound.top <= region.bottom)
	{
		return true;
	}
	return false;
}

void Quadtree::split()
{
    nodes = new Quadtree*[4];

	nodes[0] = new Quadtree(level + 1, MetroidRect(region.top,region.bottom/2,region.left,region.right/2));

	nodes[1] = new Quadtree(level + 1, MetroidRect(region.top, region.bottom / 2, region.right / 2, region.right));

	nodes[2] = new Quadtree(level + 1, MetroidRect(region.bottom / 2, region.bottom, region.left, region.right / 2));

	nodes[3] = new Quadtree(level + 1, MetroidRect(region.bottom / 2, region.bottom, region.right/2, region.right));
}

Quadtree::Quadtree()
{
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
		for (int i = 0; i < 4; i++)
		{
			nodes[i]->clear();
			delete nodes[i];
		}
		delete[] nodes;
	}

	//Clear current quadtree
	objects_list.clear();
}

void Quadtree::insert(BaseObject * obj)
{
	//insert obj into corresponding nodes
	if (nodes)
	{
		if (nodes[0]->isContain(obj->getActiveBound()))
			nodes[0]->insert(obj);
		if (nodes[1]->isContain(obj->getActiveBound()))
			nodes[1]->insert(obj);
		if (nodes[2]->isContain(obj->getActiveBound()))
			nodes[2]->insert(obj);
		if (nodes[3]->isContain(obj->getActiveBound()))
			nodes[3]->insert(obj);

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
			if (nodes[0]->isContain(objects_list.back()->getActiveBound()))
				nodes[0]->insert(objects_list.back());
			if (nodes[1]->isContain(objects_list.back()->getActiveBound()))
				nodes[1]->insert(objects_list.back());
			if (nodes[2]->isContain(objects_list.back()->getActiveBound()))
				nodes[2]->insert(objects_list.back());
			if (nodes[3]->isContain(objects_list.back()->getActiveBound()))
				nodes[3]->insert(objects_list.back());

			objects_list.pop_back();
		}
	}
}

void Quadtree::retrieve(list<BaseObject*>* return_objects_list, list<BaseObject*>* list_not_wall, list<BaseObject*>* list_wall, MetroidRect rect, BaseObject* samus)
{
	if (nodes)
	{
		if (nodes[0]->isContain(rect))
			nodes[0]->retrieve(return_objects_list, list_not_wall, list_wall, rect, samus);
		if (nodes[1]->isContain(rect))
			nodes[1]->retrieve(return_objects_list, list_not_wall, list_wall, rect, samus);
		if (nodes[2]->isContain(rect))
			nodes[2]->retrieve(return_objects_list, list_not_wall, list_wall, rect, samus);
		if (nodes[3]->isContain(rect))
		    nodes[3]->retrieve(return_objects_list, list_not_wall, list_wall, rect, samus);

		return; // Return here to ignore rest.
	}

	//Add all entities in current region into return_objects_list
	if (this->isContain(rect))
	{
		for (auto i = objects_list.begin(); i != objects_list.end(); i++)
		{
			bool found = (std::find(return_objects_list->begin(), return_objects_list->end(), *i) != return_objects_list->end());

			if (!found)
			{
				if (Collision::getInstance()->isCollide(samus->getActiveBound(), (*i)->getActiveBound()))
				{
					return_objects_list->push_back(*i);

					if ((*i)->getId() == eID::WALL)
						list_wall->push_back(*i);
				}

				if (Collision::getInstance()->isCollide(rect, (*i)->getActiveBound()))
				{
					if ((*i)->getId() != eID::WALL)
						list_not_wall->push_back(*i);
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
