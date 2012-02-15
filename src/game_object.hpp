#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "bounding_box.hpp"

class game_object {
    public:
        game_object(): bb(0,0,0,0, true) {}
        
        game_object(int x, int y, int w, int h, bool ic = true): 
            bb(x, y, w, h, ic) {}
        
        virtual ~game_object() {}
        
        virtual void draw() const = 0;
        
        void set_pos(float x, float y) { set_x(x); set_y(y); }
        void set_x(float x) { this->bb.set_x(x); }
        void set_y(float y) { this->bb.set_y(y); }
        
        float get_x() const { return this->bb.get_x(); }
        float get_y() const { return this->bb.get_y(); }
        float get_w() const { return this->bb.get_w(); }
        float get_h() const { return this->bb.get_h(); }
        
        const bounding_box& get_bounding_box() const { return this->bb; }

    protected:
        bounding_box bb;
    
    private:
        game_object(const game_object& other);
        game_object& operator=(const game_object& other);
};

#endif //GAME_OBJECT_HPP