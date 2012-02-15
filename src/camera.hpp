#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vector2d.hpp"
#include "character.hpp"

class camera {
    public:
        camera();
        ~camera();
        
        //setters
        void set_position(float x, float y);
        void set_focus(float fx, float fy);
        void set_size(float width, float height);
        void set_focused_character(character* focused_char);
        
        //getters
        vector2d get_position() const;
        vector2d get_focus() const;
        float get_width() const;
        float get_height() const;
        character* get_focused_character();
        
        void update();
    
    private:
        vector2d position;
        vector2d focus;
        float width;
        float height;
        
        character *focused_char;
    
        camera(const camera& other);
        camera& operator=(const camera& other);
};

#endif //CAMERA_HPP