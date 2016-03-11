#include "App.h"
#include "EntityManager.h"
#include "Input.h"
#include "PathFinding.h"
#include "Window.h"
#include "LegoEntity.h"
#include "p2Log.h"

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
bool EntityManager::preUpdate()
{
	if (app->input->getMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_DOWN)
	{
		iPoint p; app->input->getMousePosition(p);
		const LegoEntity *e = whichEntityOnMouse();
		if (e != NULL) remove(e->id);		
	}

	if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		selector_init = true;
		selection.clear();
		app->input->getMousePosition(initial_selector_pos);
	}

	if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT && selector_init)
	{
		app->input->getMousePosition(final_selector_pos);
		calculateSelector();
	}		

	if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
	{
		selector_init = false;
		selectAll();
	}
			
	return true;
}

// Called each loop iteration
bool EntityManager::postUpdate()
{
	// Info about the bricks
	static char title[256];
	sprintf_s(title, 256, "Active bricks: %d  Inactive bricks: %d",
	active_entities.size(), inactive_entities.size());
	app->win->setTitle(title);

	map<uint, LegoEntity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); ++it)
		it->second->draw();

	it = selection.begin();
	for (; it != selection.end(); ++it)
		app->render->DrawQuad(it->second->dim, 0, 0, 0, 255, false);

	if(selector_init) app->render->DrawQuad(selector, 255, 255, 255, 255, false);

	return true;
}

// Called before quitting
bool EntityManager::cleanUp()
{
	map<uint, LegoEntity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); it++)
	{
		delete it->second;
	}

	active_entities.clear();

	return true;
}

// Add method
LegoEntity *EntityManager::add(iPoint &pos, LEGO_TYPE type)
{
	LegoEntity *lego = NULL;
	iPoint tile_pos = app->map->worldToMap(pos.x, pos.y);

	// Checking for another bricks already on the map_tile specified by argument pos.
	map<uint, LegoEntity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); it++)
	{
		if (it->second->tile_pos == tile_pos)
			return lego; // No entity is created!
	}

	if (app->path->isWalkable(tile_pos))	// Can we add a new entity on that tile?
	{
		switch (type)
		{
		case (RED) :
			lego = new LegoRed(pos);
			break;
		case (VIOLET) :
			lego = new LegoViolet(pos);
			break;
		case (PINK) :
			lego = new LegoPink(pos);
			break;
		case (ORANGE) :
			lego = new LegoOrange(pos);
			break;
		case (GREEN) :
			lego = new LegoGreen(pos);
			break;
		case (YELLOW) :
			lego = new LegoYellow(pos);
			break;
		case (LIGHTBLUE) :
			lego = new LegoLightBlue(pos);
			break;
		case (BLUE) :
			lego = new LegoBlue(pos);
			break;
		case (DARKBLUE) :
			lego = new LegoDarkBlue(pos);
			break;
		case (LIGHTGRAY) :
			lego = new LegoLightGray(pos);
			break;
		case (GRAY) :
			lego = new LegoGray(pos);
			break;
		case (DARKGRAY) :
			lego = new LegoDarkGray(pos);
			break;
		}

		// We add the new entity to the map of active entities. 
		active_entities.insert(pair<uint, LegoEntity*>(++next_ID, lego));
		lego->id = next_ID;
		lego->tile_pos = tile_pos;
	}		
	
	return lego;
}

// Remove an entity using its ID
bool EntityManager::remove(uint id)
{
	map<uint, LegoEntity*>::iterator it = active_entities.find(id);
	
	// Whether this ID really exists, then...
	if (it != active_entities.end())
	{
		inactive_entities.insert(*it);
		active_entities.erase(id);
		selection.erase(id);				
		return true;
	}
	else
		return false;
}

// Return ID for the corresponding entity
int EntityManager::getID(const LegoEntity* entity)
{
	map<uint, LegoEntity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); it++)
	{
		if (entity == it->second) return it->first;
	}
	return -1;
}

//
LegoEntity *EntityManager::whichEntityOnMouse()
{
	iPoint p; app->input->getMousePosition(p);

	map<uint, LegoEntity*>::reverse_iterator rit = active_entities.rbegin();
	for (; rit != active_entities.rend(); ++rit)
	{
		if (p.x >= rit->second->dim.x &&
			p.x <= rit->second->dim.x + rit->second->dim.w &&
			p.y >= rit->second->dim.y &&
			p.y <= rit->second->dim.y + rit->second->dim.h)
			return rit->second;
	}
	return NULL;
}

void EntityManager::selectAll()
{
	iPoint left_top_corner = app->map->worldToMap(selector.x, selector.y);
	iPoint right_bottom_corner = app->map->worldToMap(selector.x + selector.w, selector.y + selector.h);

	for (int i = left_top_corner.x; i <= right_bottom_corner.x; i++)
	{
		for (int j = left_top_corner.y; j <= right_bottom_corner.y; j++)
		{
			map<uint, LegoEntity*>::iterator it = active_entities.begin();
			for (; it != active_entities.end(); it++)
			{
				if (it->second->tile_pos.x == i && it->second->tile_pos.y == j)
					selection.insert(pair<uint, LegoEntity*>(it->first,it->second));
			}			
		}
	}
}

void EntityManager::calculateSelector()
{
	int selector_width = abs(final_selector_pos.x - initial_selector_pos.x);
	int selector_height = abs(final_selector_pos.y - initial_selector_pos.y);
	int selector_pos_x = (initial_selector_pos.x < final_selector_pos.x ? initial_selector_pos.x : final_selector_pos.x);
	int selector_pos_y = (initial_selector_pos.y < final_selector_pos.y ? initial_selector_pos.y : final_selector_pos.y);
	selector = { selector_pos_x, selector_pos_y, selector_width, selector_height };
}