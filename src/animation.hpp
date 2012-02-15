#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "bounding_box.hpp"

class frame {
    public:
        ALLEGRO_BITMAP *image;
    
        frame(ALLEGRO_BITMAP *image): image(image) {
        }
        
        ~frame() { if (image) al_destroy_bitmap(image); }
    
    private:
        frame(const frame& other);
        frame& operator=(const frame& other);
};

typedef std::vector<frame*> collection_frames;

class animation {
    public:
        animation(float speed);
        ~animation();
        
        bool add_frame(frame *f);
        void set_speed(const float speed);
        float get_speed() const;
        void next();
        
		frame* get_frame();
		void set_frame(frame *f);
        
    private:
        float speed;                    //time between frames.
        float curr_speed;
        int current_frame;              //currently active frame
        collection_frames frames;
        
        //can't use copy constructor
        animation(const animation& other);
        
        //can't use assignment operator
        animation& operator=(const animation& other);
};

#endif //ANIMATION_HPP