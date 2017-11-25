#include "MapInfo.h"



MapInfo::MapInfo()
{
	listRectOfMapCell = new vector<RECT*>();
}


MapInfo::~MapInfo()
{
	// delete mapCell**
	if (map != NULL)
	{
		for (size_t i = 0; i < mapHeight; i++)
		{
			if (map[i] != NULL)
				delete[] map[i];
		}
		delete[] map;
	}

	// delete vector rect*
	for (std::vector< RECT* >::iterator it = listRectOfMapCell->begin(); it != listRectOfMapCell->end(); ++it)
	{
		delete (*it);
	}
	listRectOfMapCell->clear();
	delete listRectOfMapCell;
}

bool MapInfo::initialize(const char * fileJson)
{
	this->fileJson = fileJson;

	try
	{
		//read json from file
		ifstream ifs(fileJson);
		IStreamWrapper isw(ifs);
		Document jSon;
		jSon.ParseStream(isw);

		//if json isn't jsonObject throw exception 
		assert(jSon.IsObject());

		//gett width and height of map
		mapHeight = jSon["height"].GetInt();
		mapWidth = jSon["width"].GetInt();

		//new map[height][width]
		map = new MapCell*[mapHeight];
		for (int i = 0; i < mapHeight; i++)
		{
			map[i] = new MapCell[mapWidth];
		}


		//get width, height of each map cell
		tileHeight = jSon["tileheight"].GetInt();
		tileWidth = jSon["tilewidth"].GetInt();

		//get row and column of tileset
		const Value& tilesets = jSon["tilesets"];
		int column = tilesets["column"].GetInt();
		int row = tilesets["row"].GetInt();

		//list<RECT> temp;
		//init array rectangle contain map cell reference to its
		int length = column*row;
		for (size_t i = 0; i < length; i++)
		{
			RECT *rect = new RECT();
			rect->left =(LONG) (i % column) * tileWidth;
			rect->top = (LONG) (i / column) * tileHeight;
			rect->bottom = rect->top + tileHeight;
			rect->right = rect->left + tileWidth;

			listRectOfMapCell->push_back(rect);		
		}

		//assign data is a array
		assert(jSon["data"].IsArray());
		const Value& data = jSon["data"];

		for (SizeType i = 0; i < data.Size(); i++)
		{
			//get order number in data
			int y = i % mapWidth;
			int x = i / mapWidth;
			int orderNumber = data[i].GetInt();

			if (orderNumber == 0)
			{
				map[x][y].rect = NULL;
				map[x][y].visible = false;
			}
			else
			{
				map[x][y].rect = listRectOfMapCell->at(orderNumber - 1);
				map[x][y].visible = true;
			}
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

