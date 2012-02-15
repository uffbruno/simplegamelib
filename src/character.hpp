#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "game_object.hpp"
#include "sprite.hpp"

enum keycodes {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT
};

typedef enum keycodes keycodes;

class character: public game_object {
    public:
        character(float pos_x, float pos_y, float speed_x, float speed_y, 
            const std::string& directory);
        virtual ~character();
        
        //initializes sprites, mandatory step. without it anything else will work.
        void init();
        
        void set_state(const unsigned int state);
        unsigned int get_state() const { return state; }
        
        float get_speed_x() { return speed.x; }
        float get_speed_y() { return speed.y; }
        float get_curr_speed_x() { return curr_speed.x; }
        float get_curr_speed_y() { return curr_speed.y; }

        virtual void print();
        void handle_input(const ALLEGRO_EVENT& ev);
        void update();
        virtual void draw() const;

        virtual vector2d handle_collision(const game_object* other);
    
    protected:
        vector2d speed;
        vector2d curr_speed;
        sprite spr;
        
        //bounding boxes for collision detection
        bounding_box bbtop;
        bounding_box bbleft;
        bounding_box bbright;
        bounding_box bbbottom;
        
        bool loaded;
        unsigned int state;
        std::string directory; //directory that contains the frames and the anim_info.txt file.
        bool keys[4];
        
        bool load_frames();
        virtual unsigned int impl_update() = 0;
        virtual void impl_draw() const     = 0;

    private:
        void update_bounding_boxes();
        
        character(const character& other);
        character& operator=(const character& other);
};

#endif //CHARACTER_HPP