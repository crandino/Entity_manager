#include "SDL\include\SDL_render.h"
#include "Textures.h"
#include "Render.h"
#include "App.h"

enum LEGO_TYPE
{
	BADER,
	LUKE,
	LEIA,
	C3PO,
	NUM_OF_CHARACTERS
};

class LegoEntity
{


public:

	iPoint			pos;
	LEGO_TYPE		type;
	SDL_Texture		*tex;
	bool			dark_side;
	bool			robot;

	// Constructors
	LegoEntity() {};
	~LegoEntity()
	{
		SDL_DestroyTexture(tex);
	}

	void draw()
	{
		app->render->blit(tex, pos.x, pos.y);
	}

};

class Bader : public LegoEntity
{
public:

	Bader(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/bader.png");
		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
		pos.x = p.x - (w / 2);
		pos.y = p.y - (h / 2);

		robot = false;
		dark_side = true;
		type = BADER;
	}

};

class Luke : public LegoEntity
{
public:

	Luke(iPoint &p)
	{
		tex = app->tex->loadTexture("textures/luke.png");
		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
		pos.x = p.x - (w / 2);
		pos.y = p.y - (h / 2);
		
		robot = false;
		dark_side = false;
		type = LUKE;
	}

};

class Leia : public LegoEntity
{
public:

	Leia(iPoint &p) 
	{
		tex = app->tex->loadTexture("textures/leia.png");
		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
		pos.x = p.x - (w / 2);
		pos.y = p.y - (h / 2);

		robot = false;
		dark_side = false;
		type = LEIA;
	}

};

class C3po : public LegoEntity
{
public:

	C3po(iPoint &p) 
	{
		tex = app->tex->loadTexture("textures/c3po.png");
		int w, h;
		SDL_QueryTexture(tex, NULL, NULL, &w, &h);
		pos.x = p.x - (w / 2);
		pos.y = p.y - (h / 2);

		robot = true;
		dark_side = false;
		type = C3PO;
	}

};