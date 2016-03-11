#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "EntityManager.h"
#include "PathFinding.h"
#include "LegoEntity.h"
#include "Maps.h"
#include "Scene.h"

Scene::Scene() : Module()
{
	name.create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::awake(pugi::xml_node &node)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::start()
{
	app->map->load("lego_map.tmx");
	uchar *buffer = NULL;
	int width, height;
	if (app->map->createWalkabilityMap(width, height, &buffer))
		app->path->setMap(width, height, buffer);

	return true;
}

// Called each loop iteration
bool Scene::preUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::update(float dt)
{
	// All EntityManager stuff...
	
	// LEGO map drawing...
	app->map->draw();

	if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		LEGO_TYPE type = static_cast<LEGO_TYPE>(rand() % NUM_OF_BRICKS);
		iPoint p;  app->input->getMousePosition(p);
		app->entity_manager->add(p, type);
	}

	return true;
}

// Called each loop iteration
bool Scene::postUpdate()
{
	bool ret = true;

	if(app->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::cleanUp()
{
	LOG("Freeing scene");
	return true;
}