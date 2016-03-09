#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Point2d.h"

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
	bool preUpdate();

	// Called before quitting
	bool cleanUp();

private:

};

#endif // __EntityManager_H__
