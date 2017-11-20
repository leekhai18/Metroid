#include "IndexManager.h"

IndexManager* IndexManager::instance = nullptr;

IndexManager::IndexManager()
{
}

IndexManager::~IndexManager()
{
	delete instance;
}

IndexManager * IndexManager::getInstance()
{
	if (instance == nullptr)
		instance = new IndexManager();

	return instance;
}
