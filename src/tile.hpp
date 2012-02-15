#ifndef TILE_HPP
#define TILE_HPP

#include "game_object.hpp"

class tile: public game_object {
    public:
        tile(int x, int y, int w, int h, int id);
        ~tile();
        
        void is_passable(bool passable) { this->bb.set_ignore_collision(passable); }
        bool is_passable() { return this->bb.get_ignore_collision(); }
        
        void set_id(const unsigned int id) { this->tile_id = id; }
        unsigned int get_id() { return tile_id; }
        
        void draw() const;
        void draw_bounding_box() const;
    
    private:
        //ALLEGRO_BITMAP *image;
        unsigned int tile_id;
        
        tile(const tile& other);
        tile& operator=(const tile& other);
};

#endif //TILE_HPP