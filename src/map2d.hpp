#ifndef MAP2D_HPP
#define MAP2D_HPP

#include <vector>
#include <allegro5/allegro_image.h>

#include "tile.hpp"
#include "character.hpp"

typedef std::vector<tile*> cell_collection;
typedef std::vector<ALLEGRO_BITMAP*> tileset;
typedef std::vector<character*> char_collection;

class map2d {
    public:
        map2d();
        ~map2d();
        
        bool new_map();
        bool load_map(const std::string& mapname);
        bool save_map(const std::string& mapname);
        void close_map();
        bool load_tileset(const std::string& mapname, 
                          int dimension, 
                          int gap_x = 1, 
                          int gap_y = 1, 
                          int offset = 0);

        ALLEGRO_BITMAP* create_map_bitmap();
        ALLEGRO_BITMAP* create_tileset_bitmap(const unsigned int max_width) const;
        
        void draw();
        void redraw() const;
        
        void set_tile_id(unsigned int x, unsigned int y, unsigned int id);
        void set_tile_passable(unsigned int x, unsigned int y, bool passable);
        bool get_tile_passable(unsigned int x, unsigned int y);
        
        unsigned int get_width() const { return width; }
        unsigned int get_height() const { return height; }
        unsigned int get_cell_size() const { return cell_size; }
        
        void toggle_grid();
        void check_collision_with(character *g);
        
        void set_map_pos(float& x, float& y);
        void set_map_visible_size(unsigned int width, unsigned int height);
        void set_always_redraw(bool ar) { always_redraw = ar; }
        
        float get_map_posx() { return mapx; }
        float get_map_posy() { return mapy; }
        
        void add_character(character *ch) { if (ch) chars.push_back(ch); }
    
    private:
        //map info
        unsigned int rows;
        unsigned int cols;
        unsigned int width;
        unsigned int height;
        bool show_grid;
        bool must_redraw;
        bool always_redraw;
        unsigned int mapx;
        unsigned int mapy;
        unsigned int visiblewidth;
        unsigned int visibleheight;

        //tileset info
        unsigned int cell_size;
        unsigned int tile_horizontal_gap;
        unsigned int tile_vertical_gap;
        unsigned int offset;
        std::string tileset_filename;
        unsigned int default_tile_id;
        
        cell_collection cells;
        tileset tiles;
        char_collection chars;
        
        ALLEGRO_BITMAP *mapbmp;
        
        unsigned int calc_width() const;
        unsigned int calc_height() const;
    
        bool read_tileset_information(const ALLEGRO_CONFIG *config);
      
        map2d(const map2d& other);
        map2d& operator=(const map2d& other);
};

#endif //MAP2D_HPP