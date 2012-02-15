#include "tile.hpp"

tile::tile(int x, int y, int w, int h, int id): 
    game_object(x,y,w,h), tile_id(id) {
    bb.set_color(al_map_rgb(255,255,255));  
}

tile::~tile() {
}

void tile::draw() const {
}

void tile::draw_bounding_box() const {
    bb.draw();
}