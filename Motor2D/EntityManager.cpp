#include "EntityManager.h"
#include "LegoEntity.h"

EntityManager::EntityManager() : Module()
{
	name.create("EntityManager");
}

// Destructor
EntityManager::~EntityManager()
{
	next_ID = 0;
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
bool EntityManager::postUpdate()
{
	map<uint, LegoEntity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); it++)
	{
		it->second->draw();
	}

	return true;
}

// Called before quitting
bool EntityManager::cleanUp()
{
	return true;
}

// Add method
LegoEntity *EntityManager::add(iPoint &pos, LEGO_TYPE type)
{
	LegoEntity *lego = NULL;
	switch (type)
	{
	case (BADER) :
		lego = new Bader(pos);
		break;
	case (LEIA) :
		lego = new Leia(pos);
		break;
	case (LUKE) :
		lego = new Luke(pos);
		break;
	case (C3PO) :
		lego = new C3po(pos);
		break;
	}
	
	// We add the new entity to the map of active entities. Each new entity
	active_entities.insert(pair<uint, LegoEntity*>(++next_ID, lego));
	
	return lego;
}

// Return 
int EntityManager::getID(const LegoEntity* entity)
{
	map<uint, LegoEntity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); it++)
	{
		if (entity == it->second) return it->first;
	}
	return -1;
}