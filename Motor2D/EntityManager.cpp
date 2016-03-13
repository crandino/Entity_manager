#include "App.h"
#include "EntityManager.h"
#include "Input.h"
#include "PathFinding.h"
#include "Window.h"
#include "LegoEntity.h"
#include "p2Log.h"
#include <algorithm>

EntityManager::EntityManager() : Module()
{
	name.create("EntityManager");
}

// Destructor
EntityManager::~EntityManager()
{
	next_ID = 0;
	filter = 0;
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
	if (app->input->getKey(SDL_SCANCODE_0)) filter = 0;
	if (app->input->getKey(SDL_SCANCODE_1)) filter = 1;
	if (app->input->getKey(SDL_SCANCODE_2)) filter = 2;
	if (app->input->getKey(SDL_SCANCODE_3)) filter = 3;
	if (app->input->getKey(SDL_SCANCODE_4)) filter = 4;

	// Clicking middle button, eliminates an entity
	if (app->input->getMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_DOWN)
	{
		iPoint p; app->input->getMousePosition(p);
		const LegoEntity *e = whichEntityOnMouse();
		if (e != NULL) remove(e->id);		
	}

	// Clicking and holding right button, starts a selection
	if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		selector_init = true;
		selection.clear();
		selection_ordered.clear();
		app->input->getMousePosition(initial_selector_pos);
	}

	// Holding right button, updates selector dimensions
	if (selector_init && app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		app->input->getMousePosition(final_selector_pos);
		calculateSelector();
	}		

	// Once released right button, the selection is computed
	if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
	{
		selector_init = false;
		if (app->input->getKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			selectAvailableEntities(filter);
		else
			selectAll(filter);
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

	// Entities drawing
	map<uint, LegoEntity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); ++it)
		it->second->draw();

	// Basic selection. Entities surrounded by black rectangles.
	it = selection.begin();
	for (; it != selection.end(); ++it)
		app->render->DrawQuad(it->second->dim, 0, 0, 0, 255, false);

	// Drawing gradient color (red close to selection, blue for further entities) for ordered selection.
	multimap<float, LegoEntity*>::iterator ito = selection_ordered.begin();
	ito = selection_ordered.begin();
	if (selection_ordered.size() != 0)
	{
		float max_distance = selection_ordered.rbegin()->first;
		int blue, red;

		for (; ito != selection_ordered.end(); ++ito)
		{
			blue = (int)((ito->first / max_distance) * 255);
			red = 255 - blue;
			app->render->DrawQuad(ito->second->dim, red, 0, blue, 255, true);
		}
	}

	// Drawing selector (white rectangle)
	if (selector_init) app->render->DrawQuad(selector, 255, 255, 255, 255, false);

	return true;
}

// Called before quitting
bool EntityManager::cleanUp()
{
	map<uint, LegoEntity*>::iterator it = active_entities.begin();
	for (; it != active_entities.end(); it++)
		delete it->second;

	it = inactive_entities.begin();
	for (; it != inactive_entities.end(); it++)
		delete it->second;

	active_entities.clear();
	inactive_entities.clear();
	selection.clear();
	selection_ordered.clear();

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

	if (app->path->isWalkable(tile_pos))	// Can we add a new entity on that tile? i.e. Is that tile walkable?
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
	}		
	
	return lego;
}

// Remove an entity using its ID
bool EntityManager::remove(uint id)
{
	LegoEntity *e = getEntity(id);
	
	// Whether this ID really exists, then...
	if (e != NULL)
	{
		inactive_entities.insert(pair<uint, LegoEntity*>(id, e));
		active_entities.erase(id);
		selection.erase(id); 
		selection_ordered.clear();
		return true;
	}
	else
		return false;
}

// Return ID for the corresponding entity
LegoEntity *EntityManager::getEntity(uint id)
{
	map<uint, LegoEntity*>::iterator it = active_entities.find(id);
	return (it != active_entities.end() ? it->second : NULL);
}

// WhichEntityOnMouse: Returns an entity under the mouse cursor
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

void EntityManager::selectAll(uchar filter)
{
	iPoint left_top, right_bottom;
	left_top = app->map->worldToMap( selector.x, selector.y );
	right_bottom = app->map->worldToMap(selector.x + selector.w, selector.y + selector.h);

	for (uint x = left_top.x; x <= right_bottom.x; ++x)
	{
		for (uint y = left_top.y; y <= right_bottom.y; ++y)
		{
			map<uint, LegoEntity*>::iterator it = active_entities.begin();
			for (; it != active_entities.end(); it++)
			{
				if (it->second->tile_pos.x == x && it->second->tile_pos.y == y)
				{
					if (filter == 0)
						selection.insert(pair<uint, LegoEntity*>(it->first, it->second));
					else if (it->second->behaviour == filter)
						selection.insert(pair<uint, LegoEntity*>(it->first, it->second));
				}
			}
		}
	}
}

void EntityManager::selectAvailableEntities(uchar filter)
{
	pathList area;
	app->path->walkableAreaFrom(selector, area);

	doubleNode<pathNode> *item = area.list.getFirst();
	while (item)
	{
		map<uint, LegoEntity*>::iterator it = active_entities.begin();
		for (; it != active_entities.end(); it++)
		{
			if (it->second->tile_pos.x == item->data.pos.x && it->second->tile_pos.y == item->data.pos.y)
			{
				if (filter == 0)
					selection.insert(pair<uint, LegoEntity*>(it->first, it->second));
				else if (it->second->behaviour == filter)
					selection.insert(pair<uint, LegoEntity*>(it->first, it->second));
			}
		}
		item = item->next;
	}

	sortEntities();
}

// CalculateSelector: Method that computes the dimensions of the white rectangle selector.
void EntityManager::calculateSelector()
{
	int selector_width = abs(final_selector_pos.x - initial_selector_pos.x);
	int selector_height = abs(final_selector_pos.y - initial_selector_pos.y);
	int selector_pos_x = (initial_selector_pos.x < final_selector_pos.x ? initial_selector_pos.x : final_selector_pos.x);
	int selector_pos_y = (initial_selector_pos.y < final_selector_pos.y ? initial_selector_pos.y : final_selector_pos.y);
	selector = { selector_pos_x, selector_pos_y, selector_width, selector_height };
}

void EntityManager::sortEntities()
{
	iPoint middle_point = app->map->worldToMap(((selector.x + selector.w) + selector.x) / 2, ((selector.y + selector.h) + selector.y) / 2);
	map<uint, LegoEntity*>::iterator it = selection.begin();

	for (; it != selection.end(); ++it)
	{
		iPoint dest = app->map->worldToMap(it->second->dim.x, it->second->dim.y);
		float distance = app->path->costOfPath(middle_point, dest);
		selection_ordered.insert(pair<float, LegoEntity*>(distance, it->second));
	}
}