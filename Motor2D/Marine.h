#ifndef __MARINE_H__
#define __MARINE_H__

#include "Unit.h"

class Marine : public Unit
{
public:
	
	// IDLE animations
	Animation	idle_up;
	Animation	idle_right_up;
	Animation   idle_right;
	Animation	idle_right_down;
	Animation	idle_down;
	Animation	idle_left_down;
	Animation	idle_left;
	Animation	idle_left_up;
	vector<Animation*>   idle_animation_pack;
	
	// MOVING animations
	Animation	walk_up;
	Animation   walk_right_up;
	Animation   walk_right;
	Animation   walk_right_down;
	Animation   walk_down;
	Animation   walk_left_down;
	Animation   walk_left;
	Animation   walk_left_up;
	vector<Animation*>   move_animation_pack;

	// ATTACK animations
	Animation	attack_up;
	Animation	attack_right_up;
	Animation	attack_right;
	Animation	attack_right_down;
	Animation	attack_down;
	Animation	attack_left_down;
	Animation	attack_left;
	Animation	attack_left_up;
	vector<Animation*>   attack_animation_pack;

	// Dead animation
	Animation			 dead;	

	Bunker*			     bunker_to_fill = NULL;		// Bunker that can be access by the marine
	bool			     inside_bunker = false;		// It's inside?

	Timer	sound_timer;
	bool	sound_active = true;
	bool	sound_dying = true;

	// ---- Methods ----
	Marine(iPoint &p);
	~Marine();

	bool update(float dt);
	void move(float dt);
	void setAnimationFromDirection();
	void draw();
	
};

#endif __MARINE_H__