#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Point2d.h"
#include <map>

enum LEGO_TYPE;
class LegoEntity;
using namespace std;

class EntityManager : public Module
{

public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	LegoEntity *add(iPoint &pos, LEGO_TYPE type);
	void remove(uint id);
	int getID(const LegoEntity*);

private:

	map<uint, LegoEntity*> active_entities;
	map<uint, LegoEntity*> inactive_entities;
	uint next_ID;

	void drawAll();

};

#endif // __EntityManager_H__
