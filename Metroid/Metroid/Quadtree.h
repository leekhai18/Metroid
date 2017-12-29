#pragma once
#include "BaseObject.h"
#include <list>
#include <map>
#include "Constants.h"


#define MAX_LEVEL                6
#define MAX_OBJECT_IN_REGION    5


class Quadtree
{
private:
	int            level;
	MetroidRect   region;
	map<int,BaseObject*>   objects_list;
	list<Quadtree*>*     nodes;
	bool           isContain(MetroidRect bound);
	void           split();
public:
	Quadtree();

	map<int,BaseObject*>&  getObjectList();
	list<Quadtree*>*& getNodes();
	MetroidRect getRegion();
	int getLevel();

	Quadtree(int level, MetroidRect region);
	~Quadtree();
	void clear();

	void retrieve(map<int, BaseObject*>* listNotWallCanCollideSamus, map<int, BaseObject*>* listObjectNotWallOnViewPort, 
		map<int, BaseObject*>* listWallCanCollideSamus,MetroidRect rect, BaseObject * samus);
	
	//void retrieve(list<BaseObject*>* listCanCollideSamus, list<BaseObject*>* listObjectNotWallOnViewPort, list<BaseObject*>* listWallCanCollideSamus, MetroidRect rect, BaseObject* samus);


	
};