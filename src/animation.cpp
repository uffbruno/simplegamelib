#include <iostream>
#include "animation.hpp"

animation::animation(float speed): speed(speed),
    curr_speed(0), current_frame(0), frames() {
}

animation::~animation() {
    for (unsigned int i = 0; i < frames.size(); ++i) {
        delete frames[i];
	}

	frames.clear();
}

bool animation::add_frame(frame* f) {
    if (f == NULL) return false;
	if (f->image == NULL) return false;
    
    //take a pixel from background color and use it as transparency pixel
    al_convert_mask_to_alpha(f->image, al_get_pixel(f->image, 0, 0));

    //add frame to collection
    frames.push_back(f);
    
    return true;
}

void animation::set_speed(const float speed) {
    this->speed = speed;
}

float animation::get_speed() const {
    return this->speed;
}

void animation::set_frame(frame *f) { 
	if (&f != &(frames[current_frame])) {
		delete frames[current_frame];
		frames[current_frame] = f; 
	} else {
		std::cout << "pointers are equal, no need to set" << std::endl;
	}
}

void animation::next() {
    ++curr_speed;
    if (curr_speed >= speed) {
        ++current_frame;
        current_frame %= frames.size();
        curr_speed = 0;
    }
}

frame* animation::get_frame() {
    if (current_frame == -1) current_frame = 0;
    return frames.at(current_frame);
}