#include <iostream>
#include <allegro5/allegro_primitives.h>
#include "char_2d.hpp"

char_2d::char_2d(float x, float y, float sx, float sy, 
    const std::string& directory): character(x, y, sx, sy, directory), 
                                   gravity_force(0.2),
                                   jumping_force(6),
                                   vertical_speed_limit(8) {

	state = CHAR2D_JUMPING_RIGHT;
	
	// for debugging purposes
	statenames[CHAR2D_STILL_LEFT]    = "CHAR2D_STILL_LEFT";
	statenames[CHAR2D_STILL_RIGHT]   = "CHAR2D_STILL_RIGHT";
	statenames[CHAR2D_WALKING_LEFT]  = "CHAR2D_WALKING_LEFT";
	statenames[CHAR2D_WALKING_RIGHT] = "CHAR2D_WALKING_RIGHT";
	statenames[CHAR2D_JUMPING_LEFT]  = "CHAR2D_JUMPING_LEFT";
	statenames[CHAR2D_JUMPING_RIGHT] = "CHAR2D_JUMPING_RIGHT";
}

char_2d::~char_2d() {
}

unsigned int char_2d::impl_update() {
    //gravity action
	curr_speed.y += gravity_force;
	
	//limit the falling speed
    if (curr_speed.y > vertical_speed_limit) {
        curr_speed.y = vertical_speed_limit;
    }
	
	//up arrow is for jumping
	if (keys[KEY_UP]) {
        //if our character is not jumping
		if (state != CHAR2D_JUMPING_LEFT && state != CHAR2D_JUMPING_RIGHT) {
			//apply a jumping force, opposite to gravity
			//the gravity will take care of pulling our character 
			curr_speed.y = -jumping_force;
            
			//determine direction character is facing
			if (state == CHAR2D_WALKING_LEFT || state == CHAR2D_STILL_LEFT) {
                state = CHAR2D_JUMPING_LEFT;
            } 
			else if (state == CHAR2D_WALKING_RIGHT || state == CHAR2D_STILL_RIGHT) {
                state = CHAR2D_JUMPING_RIGHT;
            }
        }
    }
    
	//move left. even if in air.
    if (!keys[KEY_LEFT] && !keys[KEY_RIGHT]) {
        curr_speed.x = 0;
    }
    
    if (keys[KEY_LEFT]) {
        curr_speed.x = -speed.x;
        		
		//if on ground, change state to walking state
        if (state != CHAR2D_JUMPING_LEFT && state != CHAR2D_JUMPING_RIGHT) {
			state = CHAR2D_WALKING_LEFT;
		} else {
			state = CHAR2D_JUMPING_LEFT;
		}
	}
	
	//same for moving right
    else if (keys[KEY_RIGHT]) {
        curr_speed.x = speed.x;
        if (state != CHAR2D_JUMPING_LEFT && state != CHAR2D_JUMPING_RIGHT) {
			state = CHAR2D_WALKING_RIGHT;
		} else {
			state = CHAR2D_JUMPING_RIGHT;
		}
    } else { 
        //if no keys are pressed, character is still, and facing direction
        //he / she was originally moving
        if (state == CHAR2D_WALKING_LEFT) state = CHAR2D_STILL_LEFT;
        else if (state == CHAR2D_WALKING_RIGHT) state = CHAR2D_STILL_RIGHT;
    }
    
    bb.set_x(bb.get_x() + curr_speed.x);
    bb.set_y(bb.get_y() + curr_speed.y);
    
    return state;
}

void char_2d::print() {
	std::cout << "current state = " << statenames[state] << std::endl;
    std::cout << "bb.get_x() = " << bb.get_x() << std::endl;
    std::cout << "bb.get_y() = " << bb.get_y() << std::endl;
    std::cout << "curr_speed.x = " << curr_speed.x << std::endl;
    std::cout << "curr_speed.y = " << curr_speed.y << std::endl;
}

void char_2d::impl_draw() const {
    animation *anim = spr.get_animation();
	if (!anim) return;
	
	frame *f = anim->get_frame();
	if (!f) return;
	
   al_draw_bitmap(f->image, bb.get_x(), bb.get_y(), 0);
}

vector2d char_2d::handle_collision(const game_object* other) {
	vector2d normal = character::handle_collision(other);

	//some handling for when the character falls (collision from above)
	if (normal.y < 0) {
		//reset y speed so gravity increases when the character falls again
		if (curr_speed.y > 0) curr_speed.y = 0;
		
		//update state
		if (state == CHAR2D_JUMPING_LEFT) state = CHAR2D_STILL_LEFT;
		else if (state == CHAR2D_JUMPING_RIGHT) state = CHAR2D_STILL_RIGHT;
	} 
    // handling when character bumps his head on an obstacle (when jumping)
    else if (normal.y > 0) {
        curr_speed.y = -curr_speed.y;
    }
	
	return normal;
}