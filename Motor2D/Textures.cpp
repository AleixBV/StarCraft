#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Render.h"
#include "FileSystem.h"
#include "Textures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "Motor2D/SDL_image/libx86/SDL2_image.lib" )

Textures::Textures() : Module()
{
	name.assign("textures");
}

// Destructor
Textures::~Textures()
{}

// Called before render is available
bool Textures::awake(pugi::xml_node &node)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Textures::start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool Textures::cleanUp()
{
	LOG("Freeing textures and Image library");
	list<SDL_Texture*>::reverse_iterator item = textures.rbegin();
	for(item; item != textures.rend(); item++)
	{
		SDL_DestroyTexture(*item);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const Textures::loadTexture(const char* path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load_RW(app->fs->load(path), 1);

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool Textures::unloadTexture(SDL_Texture* texture)
{
	list<SDL_Texture*>::iterator item = textures.begin();

	for(item; item != textures.end(); item++)
	{
		if(texture == (*item))
		{
			SDL_DestroyTexture(*item);
			textures.remove(*item);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

	if(texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.push_back(texture);
	}

	return texture;
}

// Retrieve size of a texture
void Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}
