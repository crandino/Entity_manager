#include "EntityManager.h"


EntityManager::EntityManager() : Module()
{
	name.create("EntityManager");
}

// Destructor
EntityManager::~EntityManager()
{
}

// Called before render is available
bool EntityManager::awake(pugi::xml_node &node)
{
	return true;
}

// Called before the first frame
bool EntityManager::start()
{
	return true;
}

// Called each loop iteration
bool EntityManager::preUpdate()
{
	return true;
}

// Called before quitting
bool EntityManager::cleanUp()
{
	return true;
}