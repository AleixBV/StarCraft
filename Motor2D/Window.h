#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"
#include <string>

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window(bool enabled);

	// Destructor
	virtual ~Window();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before quitting
	bool cleanUp();

	// Changae title
	void setTitle(const char* new_title);

	// Retrive window size
	void getWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint getScale() const;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	std::string	   title;
	uint		   width;
	uint		   height;
	uint		   scale;
};

#endif // __WINDOW_H__
