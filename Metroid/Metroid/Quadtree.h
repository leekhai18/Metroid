#pragma once
#include "BaseObject.h"
#include <list>
#include "Constants.h"


#define MAX_LEVEL                6
#define MAX_OBJECT_IN_REGION    5


class Quadtree
{
private:
	int            level;
	MetroidRect   region;
	list<BaseObject*>   objects_list;
	list<Quadtree*>*     nodes;
	bool           isContain(MetroidRect bound);
	void           split();
public:
	Quadtree();

	list<BaseObject*>&  getObjectList();
	list<Quadtree*>*& getNodes();
	MetroidRect getRegion();
	int getLevel();

	Quadtree(int level, MetroidRect region);
	~Quadtree();
	void clear();
	void insert(BaseObject* entity);
	void retrieve(list<BaseObject*>* listCanCollideSamus, list<BaseObject*>* listObjectNotWallOnViewPort, list<BaseObject*>* listWallCanCollideSamus, MetroidRect rect, BaseObject* samus);
	static Quadtree*  createQuadTree(MetroidRect rect , list<BaseObject*>* object_list);

	
};