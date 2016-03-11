#include "SDL\include\SDL_render.h"
#include "Textures.h"
#include "Render.h"
#include "Maps.h"
#include "App.h"

enum LEGO_TYPE
{
	PINK,
	RED,
	VIOLET,
	ORANGE,
	YELLOW,
	GREEN,
	LIGHTBLUE,
	BLUE,
	DARKBLUE,
	LIGHTGRAY,
	GRAY,
	DARKGRAY,
	NUM_OF_BRICKS
};

class LegoEntity
{

public:

	SDL_Rect		dim;
	LEGO_TYPE		type;
	SDL_Texture		*tex;
	uint			id;
	iPoint			tile_pos;

	uchar			behaviour;
	/* Behaviour is being as follow:
	0 -> The entity is completely neutral. Don't attack.
	1 -> The entity attack to any other 1, 2 and above.
	2 -> Entities controled by player.
	3 -> Entities controled by computer;	
	*/

	// Constructors
	LegoEntity() {};
	// Destructor
	~LegoEntity()
	{
		SDL_DestroyTexture(tex);
	}

	void draw()
	{
		app->render->blit(tex, dim.x, dim.y);
	}

};

class LegoYellow : public LegoEntity
{
public:

	LegoYellow(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_yellow.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = YELLOW;
	}

};

class LegoGreen : public LegoEntity
{
public:

	LegoGreen(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_green.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = GREEN;
	}

};


class LegoOrange : public LegoEntity
{
public:

	LegoOrange(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_orange.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;
			
		type = ORANGE;
	}

};

class LegoLightBlue : public LegoEntity
{
public:

	LegoLightBlue(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_lightblue.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = LIGHTBLUE;
	}
};

class LegoBlue : public LegoEntity
{
public:

	LegoBlue(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_blue.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = BLUE;
	}
};

class LegoDarkBlue : public LegoEntity
{
public:

	LegoDarkBlue(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_darkblue.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = DARKBLUE;
	}
};

class LegoViolet : public LegoEntity
{
public:

	LegoViolet(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_violet.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = VIOLET;
	}

};

class LegoRed : public LegoEntity
{
public:

	LegoRed(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_red.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = RED;
	}

};

class LegoPink : public LegoEntity
{
public:

	LegoPink(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_pink.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = PINK;
	}

};

class LegoDarkGray : public LegoEntity
{
public:

	LegoDarkGray(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_darkgray.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = DARKGRAY;
	}

};

class LegoGray : public LegoEntity
{
public:

	LegoGray(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_gray.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = GRAY;
	}

};

class LegoLightGray : public LegoEntity
{
public:

	LegoLightGray(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/lego_1x1_lightgray.png");
		SDL_QueryTexture(tex, NULL, NULL, &dim.w, &dim.h);
		iPoint tmp = app->map->worldToMap(p.x, p.y);
		tmp = app->map->mapToWorld(tmp.x, tmp.y);
		dim.x = tmp.x;
		dim.y = tmp.y;

		type = LIGHTGRAY;
	}

};