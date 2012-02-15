#include "camera.hpp"

camera::camera(): position(0,0),
                  focus(0,0),
                  width(0),
                  height(0),
                  focused_char(0) {
}

camera::~camera() {
}

void camera::set_position(float x, float y) {
    position.x = x;
    position.y = y;
} 

void camera::set_focus(float fx, float fy) {
    focus.x = fx;
    focus.y = fy;
}

void camera::set_size(float width, float height) {
    this->width  = width;
    this->height = height;
}

void camera::set_focused_character(character* focused_char) {
    if (focused_char) {
        this->focused_char = focused_char;
        
        focus.x = this->focused_char->get_x();
        focus.y = this->focused_char->get_y();
    }
}

vector2d camera::get_position() const {
    return position;
}

vector2d camera::get_focus() const {
    return focus;
}

float camera::get_width() const {
    return width;
}

float camera::get_height() const {
    return height;
}

character* camera::get_focused_character() {
    return focused_char;
}
        
void camera::update() {
    int posx = focused_char->get_x() + focused_char->get_w() / 2;
    int posy = focused_char->get_y() + focused_char->get_h() / 2;
    
    int spdx = focused_char->get_curr_speed_x();
    int spdy = focused_char->get_curr_speed_y();
    
    
    if (spdy > 0) {
        if (posy > focus.y) {
            position.y += spdy;
            focus.y    += spdy;
        }
    } else {
        if (posy < focus.y) {
            position.y += spdy;
            focus.y    += spdy;
        }
    }
    
    if (spdx > 0) {
        if (posx > focus.x) {
            position.x += spdx;
            focus.x    += spdx;
        }
    } else {
        if (posx < focus.x) {
            position.x += spdx;
            focus.x    += spdx;
        }
    }
    
}
