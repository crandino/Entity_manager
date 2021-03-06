#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Point2d.h"
#include "SDL\include\SDL_rect.h"
#include <map>
#include <vector>

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
	bool preUpdate();

	// Called each loop iteration
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	LegoEntity *add(iPoint &pos, LEGO_TYPE type);
	bool remove(uint id);
	LegoEntity* getEntity(uint id);

	LegoEntity *whichEntityOnMouse();

private:

	map<uint, LegoEntity*>               active_entities;
	map<uint, LegoEntity*>             inactive_entities;
	map<uint, LegoEntity*>                     selection;
	multimap<float, LegoEntity*>       selection_ordered;
	uint next_ID;
	uchar filter;

	SDL_Rect			 selector;
	bool			selector_init;
	iPoint   initial_selector_pos;
	iPoint     final_selector_pos;

	void drawAll();
	void selectAll(uchar filter);
	void selectAvailableEntities(uchar filter);
	void calculateSelector();
	void sortEntities();

};

#endif // __EntityManager_H__
