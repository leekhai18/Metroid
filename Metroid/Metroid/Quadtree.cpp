#include "Quadtree.h"



bool Quadtree::isContain(BaseObject * obj)
{
	MetroidRect bound = obj->getActiveBound();

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
		if (nodes[0]->isContain(obj))
			nodes[0]->insert(obj);
		if (nodes[1]->isContain(obj))
			nodes[1]->insert(obj);
		if (nodes[2]->isContain(obj))
			nodes[2]->insert(obj);
		if (nodes[3]->isContain(obj))
			nodes[3]->insert(obj);

		return; // Return here to ignore rest.
	}
	//Insert obj into current quadtree
	if (this->isContain(obj))
	{
		objects_list.push_back(obj);
	}
	//Split and move all objects in list into it’s corresponding nodes
	if (objects_list.size() > MAX_OBJECT_IN_REGION && level < MAX_LEVEL)
	{
		split();

		while (!objects_list.empty())
		{
			if (nodes[0]->isContain(objects_list.back()))
				nodes[0]->insert(objects_list.back());
			if (nodes[1]->isContain(objects_list.back()))
				nodes[1]->insert(objects_list.back());
			if (nodes[2]->isContain(objects_list.back()))
				nodes[2]->insert(objects_list.back());
			if (nodes[3]->isContain(objects_list.back()))
				nodes[3]->insert(objects_list.back());

			objects_list.pop_back();
		}
	}
}

void Quadtree::retrieve(list<BaseObject*>* return_objects_list, BaseObject * obj)
{
	if (nodes)
	{
		if (nodes[0]->isContain(obj))
			nodes[0]->retrieve(return_objects_list, obj);
		if (nodes[1]->isContain(obj))
			nodes[1]->retrieve(return_objects_list, obj);
		if (nodes[2]->isContain(obj))
			nodes[2]->retrieve(return_objects_list, obj);
		if (nodes[3]->isContain(obj))
		    nodes[3]->retrieve(return_objects_list, obj);

		return; // Return here to ignore rest.
	}

	//Add all entities in current region into return_objects_list
	if (this->isContain(obj))
	{
		for (auto i = objects_list.begin(); i != objects_list.end(); i++)
		{
			if (obj != *i)
				return_objects_list->push_back(*i);
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
