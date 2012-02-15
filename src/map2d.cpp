#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "map2d.hpp"

map2d::map2d(): rows(0), 
                cols(0), 
                width(0), 
                height(0), 
                show_grid(false),
                must_redraw(false),
                always_redraw(false),
                mapx(0),
                mapy(0),
                visiblewidth(0),
                visibleheight(0),
                cell_size(0), 
                tile_horizontal_gap(0), 
                tile_vertical_gap(0), 
                offset(0), 
                tileset_filename(), 
                default_tile_id(0),  
                cells(), 
                tiles(),
                chars(),
                mapbmp(0)
{
}

map2d::~map2d() {
    this->close_map();
}

bool map2d::new_map() {
    //closes previously loaded map.
    close_map();
    
    //tileset
    tileset_filename    = "resources//tilesets//default.png";
    cell_size           = 32;
    tile_horizontal_gap = 2;
    tile_vertical_gap   = 2;
    offset              = 2;

    if (!load_tileset(tileset_filename, cell_size, tile_horizontal_gap, tile_vertical_gap, offset)) {
        std::cout << "error loading tileset " << tileset_filename << std::endl;
        return false;
    }
    
    //map
    rows  = 32;
    cols  = 32;
    default_tile_id = 160;
    
    //create default cells
    int pos_y = 0;
    for (unsigned int i = 0; i < rows; ++i) {
        int pos_x = 0;
        for (unsigned int j = 0; j < cols; ++j) {
            tile *t = new tile(pos_x, pos_y, cell_size, cell_size, default_tile_id);
            t->is_passable(true);
            cells.push_back(t);
            
            pos_x += cell_size;
        }
        pos_y += cell_size;
    }
    
    //calculate dimensions and create map bitmap.
    width = calc_width();
    height = calc_height();
    mapbmp = al_create_bitmap(width, height);
    
    return true;
}

void map2d::close_map() {
    cells.clear();
    for (unsigned int i = 0; i < tiles.size(); ++i) {
        if (tiles[i]) al_destroy_bitmap(tiles[i]);
        tiles.clear();
    }
    
    if (mapbmp) al_destroy_bitmap(mapbmp);
}

bool map2d::load_map(const std::string& mapname) {
    //closes previously loaded map
    close_map();

    //load map
    ALLEGRO_CONFIG *map_config = al_load_config_file(mapname.c_str());
    if (!map_config) {
        std::cout << "error loading config file for map" << std::endl;
        return false;
    }
    
    if (!read_tileset_information(map_config)) {
        std::cout << "error reading tileset information" << std::endl;
        return false;
    };
    
    const char *number_of_rows = al_get_config_value(map_config, "map", "number_of_rows");
    if (!number_of_rows) {
        std::cout << "number_of_rows key doesn't exist" << std::endl;
        return false;
    }
    
    const char *number_of_cols = al_get_config_value(map_config, "map", "number_of_cols");
    if (!number_of_cols) {
        std::cout << "number_of_cols key doesn't exist" << std::endl;
        return false;
    }
    
    const char *default_tileid = al_get_config_value(map_config, "map", "default_tile_id");
    
    rows            = std::atoi(number_of_rows);
    cols            = std::atoi(number_of_cols);
    default_tile_id = std::atoi(default_tileid);
    
    if (!rows || !cols) {
        std::cout << "invalid row / column value" << std::endl;
        return false;
    }
    
    int pos_y = 0;
    
    // read each cell's data
    for (unsigned int i = 0; i < rows; ++i) {
        int pos_x = 0;
        for (unsigned int j = 0; j < cols; ++j) {
            std::ostringstream oss_cell;
            oss_cell << "cell " << i << " " << j;
            
            // tile id
            const char *sid = al_get_config_value(map_config, oss_cell.str().c_str(), "id");
            int id = default_tile_id;
            if (sid) {
                id = std::atoi(sid);
            }
            
            //if the tile is passable (i.e. collision detection is ignored)
            const char *spassable = al_get_config_value(map_config, oss_cell.str().c_str(), "passable");
            bool passable = true;
            if (spassable) {
                passable = std::atoi(spassable) ? true : false;
            }
            
            //create new tile
            tile *t = new tile(pos_x, pos_y, cell_size, cell_size, id);
            t->is_passable(passable);
            cells.push_back(t);
            
            pos_x += cell_size;
        }
        pos_y += cell_size;
    }
    
    al_destroy_config(map_config);
    
    width = calc_width();
    height = calc_height();
    
    mapbmp = create_map_bitmap();
    
    return true;
}

void map2d::set_map_pos(float& x, float& y) {
    if (x < 0) x = 0;
    else if (x + visiblewidth > width) x = width - visiblewidth;
    if (y < 0) y = 0;
    else if (y + visibleheight > height) y = height - visibleheight;
    
    mapx = x;
    mapy = y;
}

void map2d::set_map_visible_size(unsigned int width, unsigned int height) {
    visiblewidth = width;
    visibleheight = height;
}

bool map2d::save_map(const std::string& mapname) {
    ALLEGRO_CONFIG* config = al_create_config();
    
    const int max_chars = 100;
    char *values = new char[max_chars];
    
    //tileset information
    al_add_config_section(config, "tileset");
    al_set_config_value(config, "tileset", "tileset_file"       , tileset_filename.c_str());
    
    std::sprintf(values, "%d", cell_size);
    al_set_config_value(config, "tileset", "tile_dimension"     , values);
    
    std::sprintf(values, "%d", tile_horizontal_gap);
    al_set_config_value(config, "tileset", "tile_horizontal_gap", values);
    
    std::sprintf(values, "%d", tile_vertical_gap);
    al_set_config_value(config, "tileset", "tile_vertical_gap"  , values);
    
    std::sprintf(values, "%d", offset);
    al_set_config_value(config, "tileset", "offset"             , values);
    
    //map metadata
    al_add_config_section(config, "map");
    
    std::sprintf(values, "%d", rows);
    al_set_config_value(config, "map", "number_of_rows", values);
    
    std::sprintf(values, "%d", cols);
    al_set_config_value(config, "map", "number_of_cols", values);
    
    std::sprintf(values, "%d", default_tile_id);
    al_set_config_value(config, "map", "default_tile_id", values);
    
    //map data
    char *cellcat = new char[max_chars];
    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            unsigned int tile_id = cells[i * cols + j]->get_id();

            if (tile_id != default_tile_id) {
                std::sprintf(cellcat, "cell %d %d", i, j);
                std::sprintf(values, "%d", cells[i * cols + j]->get_id());
                al_set_config_value(config, cellcat, "id", values);
                
                std::sprintf(values, "%d", cells[i * cols + j]->is_passable());
                al_set_config_value(config, cellcat, "passable", values);
            }
        }
    }
    
    delete [] values;
    delete [] cellcat;
    
    if (!al_save_config_file(mapname.c_str(), config)) {
        std::cout << "error saving " << mapname << std::endl;
        return false;
    }
    
    al_destroy_config(config);
    return true;
}

bool map2d::read_tileset_information(const ALLEGRO_CONFIG *config) {
    const char* tileset_file = al_get_config_value(config, "tileset", "tileset_file");
    if (!tileset_file) {
        std::cout << "tileset_file key doesn't exist" << std::endl;
        return false;
    }
    
    const char *dimension = al_get_config_value(config, "tileset", "tile_dimension");
    if (!dimension) {
        std::cout << "tile_dimension key doesn't exist" << std::endl;
        return false;
    }
    
    //gap_x and gap_y are optional keys.
    const char *gap_x = al_get_config_value(config, "tileset", "tile_horizontal_gap");
    if (!gap_x) {
        std::cout << "tile_horizontal_gap key doesn't exist" << std::endl;
        gap_x = new char[2];
        gap_x = "1";
    }

    const char *gap_y = al_get_config_value(config, "tileset", "tile_vertical_gap");
    if (!gap_y) {
        std::cout << "tile_vertical_gap key doesn't exist" << std::endl;
        gap_y = new char[2];
        gap_y = "1";
    }
    
    const char *offset = al_get_config_value(config, "tileset", "offset");
    if (!offset) {
        std::cout << "offset key doesn't exist" << std::endl;
        offset = new char[2];
        offset = "0";
    }

    return this->load_tileset(tileset_file, 
                              std::atoi(dimension), 
                              std::atoi(gap_x), 
                              std::atoi(gap_y), 
                              std::atoi(offset));
}

bool map2d::load_tileset(const std::string& imgname, int cell_dimension, int gap_x, int gap_y, int offset) {
                         
    ALLEGRO_BITMAP *tileimg = al_load_bitmap(imgname.c_str());
    if (!tileimg) {
        std::cout << "tileset file not found: " << imgname << std::endl;
        return false;
    }
    
    this->tile_horizontal_gap = gap_x;
    this->tile_vertical_gap = gap_y;
    this->offset = offset;
    this->cell_size = cell_dimension;
    this->tileset_filename.assign(imgname.c_str());
    
    std::cout << "loading " << imgname << std::endl;
    
    unsigned int pos_x         = this->offset;
    unsigned int pos_y         = this->offset;
    unsigned int bitmap_width  = al_get_bitmap_width(tileimg);
    unsigned int bitmap_height = al_get_bitmap_height(tileimg);
    
    while (pos_y < bitmap_height) {
        if (pos_x + cell_size < bitmap_width) {
            ALLEGRO_BITMAP *tile = al_clone_bitmap(al_create_sub_bitmap(tileimg, 
                                                                        pos_x, 
                                                                        pos_y, 
                                                                        cell_size,
                                                                        cell_size));
        
            if (!tile) {
                std::cout << "error cloning tile" << std::endl;
                return false;
            }
        
            tiles.push_back(tile);
            pos_x += cell_size + tile_horizontal_gap;
        } else {
            pos_x = this->offset;
            pos_y += cell_size + tile_vertical_gap;
        }
    }
    
    al_destroy_bitmap(tileimg);
    return true;
}

unsigned int map2d::calc_width() const {
    return cell_size * cols;
}

unsigned int map2d::calc_height() const {
    return cell_size * rows;
}

void map2d::redraw() const {
    ALLEGRO_BITMAP *old = al_get_target_bitmap();
    
    al_set_target_bitmap(mapbmp);
    
    al_clear_to_color(al_map_rgb(0,0,0));
    
    for (unsigned int i = 0; i < cells.size(); ++i) {
        tile *t = cells[i];
        al_draw_bitmap(tiles[t->get_id()], t->get_x(), t->get_y(), 0);

        if (show_grid) {
            cells[i]->draw_bounding_box();
        }
    }
    
    for (unsigned int i = 0; i < chars.size(); ++i) {
        chars[i]->draw();
    }
    
    al_set_target_bitmap(old);
}

void map2d::draw() {
    if (always_redraw) {
        redraw();
    } else if (must_redraw) {
        redraw();
        must_redraw = false;
    }
    
    al_draw_bitmap_region(mapbmp, mapx, mapy, visiblewidth, visibleheight, 0, 0, 0);
}

void map2d::set_tile_id(unsigned int x, unsigned int y, unsigned int id) {
    if (x > width || y > height || id > tiles.size()) return;
    
    unsigned int row = (y / cell_size);
    unsigned int col = (x / cell_size);
    
    tile *t = cells.at(row * cols + col);
    t->set_id(id);
    must_redraw = true;
}

void map2d::set_tile_passable(unsigned int x, unsigned int y, bool passable) {
    if (x > width || y > height) 
        return;
    
    unsigned int row = (y / cell_size);
    unsigned int col = (x / cell_size);
    
    tile *t = cells.at(row * cols + col);
    t->is_passable(passable);
    must_redraw = true;
}

bool map2d::get_tile_passable(unsigned int x, unsigned int y) {
    if (x > width || y > height) 
        return false;
    
    unsigned int row = (y / cell_size);
    unsigned int col = (x / cell_size);
    
    tile *t = cells.at(row * cols + col);
    return t->is_passable();
}

ALLEGRO_BITMAP* map2d::create_tileset_bitmap(const unsigned int max_width) const {
    unsigned int cols = std::ceil(static_cast<double>(max_width) / cell_size);
    unsigned int width = cols * cell_size;
    
    if (width > max_width) {
        width -= cell_size;
        cols = std::ceil(static_cast<double>(width) / cell_size);   
    }
    
    unsigned int rows = std::ceil(static_cast<double>(tiles.size()) / cols);
    unsigned int height = rows * cell_size;

    if (width == 0 || height == 0) return 0;
    
    ALLEGRO_BITMAP *bmp = al_create_bitmap(width, height);
    ALLEGRO_BITMAP *old = al_get_target_bitmap();
    
    al_set_target_bitmap(bmp);

    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int size = tiles.size();
    unsigned int index = 0;

    while(index < size) {
        al_draw_bitmap(tiles.at(index), j * cell_size, i * cell_size, 0);
        ++j;
        if (j >= cols) {
            j = 0;
            ++i;
        }
        ++index;
    }

    al_set_target_bitmap(old);
    return bmp;
}

void map2d::check_collision_with(character *c) {
    if (c == NULL) return;
    for (unsigned int i = 0; i < cells.size(); ++i) {
        c->handle_collision(cells[i]);
    }
}

ALLEGRO_BITMAP* map2d::create_map_bitmap() {
    int width  = get_width();
    int height = get_height();

    ALLEGRO_BITMAP *bmp = al_create_bitmap(width, height);
    ALLEGRO_BITMAP *old = al_get_target_bitmap();
    
    al_set_target_bitmap(bmp);

    for (unsigned int index = 0; index < cells.size(); ++index) {
        tile *t = cells[index];
        al_draw_bitmap(tiles[t->get_id()], t->get_x(), t->get_y(), 0);
        if (show_grid) {
            cells[index]->draw_bounding_box();
        }
    }

    al_set_target_bitmap(old);
    al_save_bitmap("teste.png", bmp);
    return bmp;
}

void map2d::toggle_grid() {
    show_grid = !show_grid;
    must_redraw = true;
}