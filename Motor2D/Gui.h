#ifndef __GUI_H__
#define __GUI_H__

#include "App.h"
#include "Module.h"
#include "Render.h"
#include "Animation.h"
#include "EntityManager.h"

class GuiCursor;
class GuiImage;
class GuiLabel;
class GuiMinimap;
class GuiInfo;
class GuiTimer;
class GuiResources;
class GuiPortrait;
using namespace std;


enum GUI_EVENTS
{
	LISTENING_ENDS,
	MOUSE_ENTERS,
	MOUSE_LEAVES,
	MOUSE_LCLICK_DOWN,
	MOUSE_LCLICK_DOWN_REPEAT,
	MOUSE_LCLICK_UP,
	MOUSE_RCLICK_DOWN,
	MOUSE_RCLICK_UP,
	GAIN_FOCUS,
	LOST_FOCUS,
    INPUT_CHANGED,
	INPUT_SUBMIT,
    VALUE_CHANGED,
	SAVE_POS_THUMB,
	LOAD_POS_THUMB
};

enum GUI_TYPES
{
	IMAGE,
	CURSOR,
	LABEL,
	INFO,
	MINIMAP,
	TIMER,
	RESOURCES,
	PORTRAIT

};

// ---------------------------------------------------
class Gui : public Module
{
public:

	Gui(bool enabled);

	// Destructor
	~Gui();

	// Called when before render is available
	bool awake(pugi::xml_node&);

	// Call before first frame
	bool start();

	// Called before all Updates
	bool preUpdate();

	// Called each loop iteration
	bool update(float dt);

	// Called after all Updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	bool load(pugi::xml_node &node);
	bool save(pugi::xml_node &node) const;

	void disableHUDelements();

	// Gui creation functions
	GuiImage* createImage(const char* filename);
	GuiImage* createImage(const SDL_Texture* texture, const SDL_Rect& atlas_section);
	GuiImage createImage(GuiImage* image, iPoint position);
	GuiCursor* createCursor(const SDL_Texture* texture);
	GuiLabel* createLabel(const char* text, int num_of_font = 0);
	GuiMinimap* createMinimap(SDL_Rect rect, const char *pathTex);
	GuiTimer* createTimer(iPoint pos, const char *pathTex, Timer &timer_associated);
	GuiInfo* createInfo(iPoint pos, const char *tex_path);
	GuiResources* createResourceInfo(const char* _entity_name, int _mineral, int _gas,iPoint pos,bool draw_element = false);
	GuiResources* createResourceInfo(const char* _entity_name, iPoint pos, bool only_info, bool draw_element = false);
	GuiPortrait* createPortrait(const SDL_Texture* texture, Animation animation);

	const GuiElements* findMouseHover();
	const SDL_Texture* getAtlas() const;

	void onGui(GuiElements* ui, GUI_EVENTS event);

	// Variables for Cursor-------------------
	GuiCursor *cursor;
	iPoint cursor_offset;
	iPoint map_limits;
	float scroll_speed;
	fPoint last_attack_position;
	list<fPoint> lasts_attack_position;
	Timer timer_to_ping_attack;

	//Sounds
	unsigned int fx_click_1;
	unsigned int fx_click_error;
	unsigned int fx_not_enough_minerales;

	//HUD Ingame----------------------------- 
	GuiImage* ui_terran = nullptr;
	GuiImage* rectangle_map = nullptr;
	GuiImage* rectangle_command = nullptr;
	GuiImage* rectangle_command_2 = nullptr;
	GuiImage* rectangle_command_3 = nullptr;
	GuiImage* rectangle_command_4 = nullptr;
	GuiImage* rectangle_command_5 = nullptr;
	GuiImage* rectangle_command_6 = nullptr;
	GuiImage* rectangle_command_7 = nullptr;
	GuiImage* rectangle_command_8 = nullptr;
	GuiImage* rectangle_command_9 = nullptr;
	
	//Command Center HUD--------------------
	GuiImage* ui_create_bot = nullptr;
	GuiImage* ui_create_builds = nullptr;
	GuiImage* ui_create_bunker = nullptr;
	GuiImage* ui_create_factory = nullptr;
	GuiImage* ui_create_barraks = nullptr;

	//Control Barracks and Factories
	bool barrackAlive = false;
	bool factoryAlive = false;

	//Barracks HUD-------------------------
	GuiImage* ui_create_marine = nullptr;
	GuiImage* ui_create_medic = nullptr;
	GuiImage* ui_create_firebat = nullptr;
	GuiImage* ui_set_bomb = nullptr;

	//Factory HUD-----------------------
	GuiImage* ui_create_tank = nullptr;
	GuiImage* ui_normal_tank = nullptr;
	GuiImage* ui_siege_tank = nullptr;

	bool buildingMenuOpened = false;
	bool commandCenterOpened = false;
	bool barrackMenuOpened = false;
	bool factoryMenuOpened = false;
	bool show_wireframe_bomb = false;

	void openBuildingMenu();
	void controlIconsSprite();

	//Wireframes and Info
	//Wireframes
	SDL_Texture* atlas_wireframes = nullptr;

	// Bomb Wireframe
	GuiImage* bomb_wireframe = nullptr;

	//Bunkers
	GuiLabel* bunkerName = nullptr;
	GuiImage* bunkerWireframe = nullptr;
	GuiLabel* bunkerInfo = nullptr;
	GuiLabel* bunkerInfo2 = nullptr;
	//Command Center
	GuiLabel* commandCenterName = nullptr;
	GuiImage* commandCenterWireframe = nullptr;
	GuiLabel* commandCenterInfo = nullptr;
	//Factory
	GuiLabel* factoryName = nullptr;
	GuiImage* factoryWireframe = nullptr;
	GuiLabel* factoryInfo = nullptr;
	//Barracks
	GuiLabel* barrackName = nullptr;
	GuiImage* barrackWireframe = nullptr;
	GuiLabel* barrackInfo = nullptr;
	//Marine
	GuiImage* marineWireframe = nullptr;
	//Medic
	GuiImage* medicWireframe = nullptr;
	//Firebat
	GuiImage* firebatWireframe = nullptr;
	//Tank
	GuiImage* tankWireframe = nullptr;
	//Raynor
	GuiImage* raynorWireframe = nullptr;
	//SCV
	GuiImage* scvWireframe = nullptr;

	//Small Wireframes
	//Marine
	GuiImage* smallMarineWireframe = nullptr;
	//Medic
	GuiImage* smallMedicWireframe = nullptr;
	//Firebat
	GuiImage* smallFirebatWireframe = nullptr;
	//Tank
	GuiImage* smallTankWireframe = nullptr;
	//Raynor
	GuiImage* smallRaynorWireframe = nullptr;
	//SCV
	GuiImage* smallScvWireframe = nullptr;

	void showWireframesSelection();
	void wireframeType(SPECIALIZATION type, uint position);
	void enableWireframesSelection(bool active);

	//List of wireframes for selection
	list<GuiImage> selection_wireframes;

	//Portraits----------------------------
	GuiImage* hide_portrait = nullptr;
	//Portrait Marine----------------------
	SDL_Texture* marine_portrait_tex = nullptr;
	GuiPortrait* marine_portrait = nullptr;
	Animation marine_portrait_anim;
	//Portrait Medic----------------------
	SDL_Texture* medic_portrait_tex = nullptr;
	GuiPortrait* medic_portrait = nullptr;
	Animation medic_portrait_anim;
	//Portrait Firebat----------------------
	SDL_Texture* firebat_portrait_tex = nullptr;
	GuiPortrait* firebat_portrait = nullptr;
	Animation firebat_portrait_anim;
	//Portrait SCV----------------------
	SDL_Texture* scv_portrait_tex = nullptr;
	GuiPortrait* scv_portrait = nullptr;
	Animation scv_portrait_anim;
	//Portrait Tank----------------------
	SDL_Texture* tank_portrait_tex = nullptr;
	GuiPortrait* tank_portrait = nullptr;
	Animation tank_portrait_anim;
	//-------------------------------------

	//Bunkers HUD---------------------------
	GuiImage* ui_leave_bunker = nullptr;
	list<Bunker*> bunker_to_leave;
	bool findBunkerToLeave(Bunker* bunker);

	//HUD Label-----------------------------
	GuiLabel* number_of_wave = nullptr;

	//HUD  Mineral and Gass-----------------
	GuiImage* ui_mineral = nullptr;
	GuiLabel* number_of_minerals = nullptr;
	GuiImage* ui_gas = nullptr;
	GuiLabel* number_of_gass = nullptr;

	//HUD Info SCV and Bunker-----------------
	GuiResources* info_bunker2 = nullptr;
	GuiResources* info_barraks = nullptr;
	GuiResources* info_factory = nullptr;

	GuiResources* info_scv2 = nullptr;
	GuiResources* info_marine = nullptr;
	GuiResources* info_firebat = nullptr;
	GuiResources* info_medic = nullptr;
	GuiResources* info_tank = nullptr;

	GuiResources* info_building_menu = nullptr;
	GuiResources* info_close_building_menu = nullptr;
	GuiResources* info_tank_to_siege = nullptr;
	GuiResources* info_tank_to_move = nullptr;

	// HUD Graphic Timers
	GuiTimer *wave_timer;

	// HUD Minimap
	GuiMinimap* mini_map;

	//Raynor indicator Image
	GuiImage* raynor_indicator = nullptr;

	// HUD Menu Scene
	GuiImage *background, *start_button, *close_button, *ok_controls_button, *controls_button, *controls_panel;

	//Variables for CircleSelection. There are 10 size selection
	/*W_22 = { 3, 4, 22, 13 }; Marine, Zergling
	W_32 = { 28, 9, 32, 19 }; Mutalisk
	W_48 = { 64, 5, 48, 29 };
	W_62 = { 116, 2, 62, 37}; Tank
	W_72 = { 184, 2, 72, 43};
	W_94 = { 261, 1, 94, 56};
	W_94 = { 261, 1, 94, 56};
	W_110 = { 361, 1, 110, 66};
	W_122 =  {376, 74, 122, 73};
	W_146 = {226, 66, 146, 88};
	W_222 = { 2, 44, 222, 134};*/
	SDL_Texture*	circles_of_selection;

	// Variables for life of entities;
	SDL_Texture* lifes_tex;
	SDL_Rect	 green_life;
	SDL_Rect	 yellow_life;
	SDL_Rect	 red_life;
	SDL_Rect     white_life;	

private:

	bool debug = false;
	list<GuiElements*> elements;
	const GuiElements* focus = nullptr;
	SDL_Texture* atlas = nullptr;
	SDL_Texture* terran_console = nullptr;
	string atlas_file_name;
	void drawHudSelection(SPECIALIZATION  selection);

	SDL_Texture *path_walkability;
};

#endif