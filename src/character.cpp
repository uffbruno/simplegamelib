#include <sstream>
#include <fstream>
#include <iostream>

#include "character.hpp"

character::character(float pos_x, float pos_y, float speed_x, float speed_y, 
    const std::string& directory): game_object(pos_x, pos_y, 0, 0, false),
                                   speed(speed_x, speed_y),
                                   curr_speed(0,0),
                                   spr(),
                                   bbtop(0,0,0,0, false),
                                   bbleft(0,0,0,0, false),
                                   bbright(0,0,0,0, false),
                                   bbbottom(0,0,0,0, false),
                                   loaded(false), 
                                   state(0), 
                                   directory(directory.c_str()) {
    keys[KEY_LEFT]  = false; 
    keys[KEY_RIGHT] = false;
    keys[KEY_UP]    = false;
    keys[KEY_DOWN]  = false;
}

character::~character() {
}

void character::init() {
    if (!loaded) {
        loaded = load_frames();
        update_bounding_boxes();
    }
}

bool character::load_frames() {
    std::ostringstream oss;
    oss << directory << "//anim_info.txt";

    std::ifstream file(oss.str().c_str());
  
    if (!file) {
        std::cout << "error opening " << oss.str().c_str() << std::endl;
        return false;
    }
    
    std::string line;
    animation *anim = NULL;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#' || line[0] == '\n') {
            continue;
        } else if (line[0] == '.') {
            if (anim != NULL) {
                spr.add_animation(anim);
            }
            
            anim = NULL;
        } else {
            std::ostringstream oss_frame;        
            oss_frame << directory << "//" << line;

            if (anim == NULL) {
                anim = new animation(8.0);
            }

            std::cout << "opening " << oss_frame.str().c_str() << std::endl;
            ALLEGRO_BITMAP *image = al_load_bitmap(oss_frame.str().c_str());
            if (image == NULL) {
                std::cout << "error loading frame" << std::endl;
            } else {
                frame *f = new frame(image);
                anim->add_frame(f);
            }
        }
    }
    
    file.close();
    return true;
}

vector2d character::handle_collision(const game_object* other) {
    if (other == NULL) return vector2d(0,0);
    
    vector2d normal(0,0);

    if (    bbtop.collide   (other->get_bounding_box(), normal) ||
            bbbottom.collide(other->get_bounding_box(), normal) ||
            bbleft.collide  (other->get_bounding_box(), normal)   ||
            bbright.collide (other->get_bounding_box(), normal)) {
            
        bb.set_x(this->bb.get_x() + normal.x);
        bb.set_y(this->bb.get_y() + normal.y);
    } 

    return normal;
}

void character::set_state(const unsigned int state) {
    unsigned int num_animations = spr.get_num_animations();
    if (state > num_animations) {
        this->state = 0;
    }
    else {
        this->state = state;
    }
    
    spr.set_animation(state);
    update_bounding_boxes();
}

void character::handle_input(const ALLEGRO_EVENT& ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                keys[KEY_UP] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
            case ALLEGRO_KEY_DOWN:
                keys[KEY_DOWN] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
            case ALLEGRO_KEY_LEFT:
                keys[KEY_LEFT] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[KEY_RIGHT] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
        }
    }
}

void character::update_bounding_boxes() {
    animation *anim = spr.get_animation();
    frame *f = anim->get_frame();
    
    bb.set_w(al_get_bitmap_width(f->image));
    bb.set_h(al_get_bitmap_height(f->image));
    
    //updates bounding boxes' positions and sizes
    //to match those of the image.
    bbtop.set_x(bb.get_x());
    bbtop.set_y(bb.get_y());
    bbtop.set_w(bb.get_w());
    bbtop.set_h(1);
    
    bbleft.set_x(bb.get_x());
    bbleft.set_y(bb.get_y());
    bbleft.set_w(1);
    bbleft.set_h(bb.get_h());
    
    bbright.set_x(bb.get_x() + bb.get_w());
    bbright.set_y(bb.get_y());
    bbright.set_w(1);
    bbright.set_h(bb.get_h());
    
    bbbottom.set_x(bb.get_x());
    bbbottom.set_y(bb.get_y() + bb.get_h());
    bbbottom.set_w(bb.get_w());
    bbbottom.set_h(1);
}

void character::update() {
    if (loaded) {
        state = impl_update();
        
        //animates the character
        spr.set_animation(state);
        spr.run_animation();
        update_bounding_boxes();
    }
}

void character::print() {
    std::cout << "center.x      = " << bb.get_x() << std::endl;
    std::cout << "center.y      = " << bb.get_y() << std::endl;
}

void character::draw() const {
    if (loaded) {
        impl_draw();
    }
}