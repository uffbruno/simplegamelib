#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "bounding_box.hpp"

bounding_box::bounding_box(float x, float y, float w, float h, bool ic):
    position(x, y), w(w), h(h), ignore_collision(ic), color() {

    ALLEGRO_COLOR c = ignore_collision ? al_map_rgb(255,255,255) : al_map_rgb(255,0,0);
    this->color = c;
}

bounding_box::~bounding_box() {
}

void bounding_box::draw() const {
    al_draw_rectangle(position.x, position.y, position.x + w, position.y + h, color, 0);
}

/*-----------------------------------------------------------------------------------------------

I have adapted this algorithm from http://forums.elysianshadows.com/viewtopic.php?f=13&t=5767
(thanks to the user N64vSNES). I'm still struggling to understand the check on the if that's 
indicated with the [NO UNDERSTAND!!!] but i guess this works fine. :-)
All comments on this algorithm were made by N64vSNES on his thread post (link above).

-------------------------------------------------------------------------------------------------*/
bool bounding_box::collide(const bounding_box& other, vector2d& normal) const {
    
    //if the flag to ignore collisions is on, we don't need to calculate anything
    if (this->ignore_collision) return false;
    if (other.ignore_collision) return false;
    
    // The distance between the two objects
    vector2d distance(0,0);
   
    // The absDistance between the objects
    vector2d abs_distance(0,0);

    float center_x1 = this->position.x + this->w/2;
    float center_y1 = this->position.y + this->h/2;
    float center_x2 = other.position.x + other.w/2;
    float center_y2 = other.position.y + other.h/2;

    // Calculate the distance between A and B
    distance.x = center_x2 - center_x1;
    distance.y = center_y2 - center_y1;

    // Combine both rectangles and half the returned value
    float xadd = (other.w + this->w) / 2.0f;
    float yadd = (other.h + this->h) / 2.0f;

    // Get absolute value of distance vector.
    abs_distance.x = (distance.x < 0.0f) ? -distance.x : distance.x;
    abs_distance.y = (distance.y < 0.0f) ? -distance.y : distance.y;

    //[NO UNDERSTAND!!!]
    /*If the absDistance X is less than X add and the absDistance is less thank YAdd
    then it dosen't take a genius to figure out they arn't colliding so return false*/
    if( ! ( ( abs_distance.x < xadd ) && ( abs_distance.y < yadd ) ) ) {   
        return false;
    }
    //[END OF NO UNDERSTAND!!!]

    /*Get the magnitute by the overlap of the two rectangles*/
    float xmag = xadd - abs_distance.x;
    float ymag = yadd - abs_distance.y;

    /*Determine what axis we need to act on based on the overlap*/
    if(xmag < ymag) {
      normal.x = (distance.x > 0) ? -xmag : xmag;
    }
    else if (xmag > ymag) {
      normal.y = (distance.y > 0) ? -ymag : ymag;
   }
   
   // If we reached this point then we now know there was a collision
   return true;
}

void bounding_box::set_ignore_collision(bool ignore_collision) {
    this->ignore_collision = ignore_collision;
    ALLEGRO_COLOR c = ignore_collision ? al_map_rgb(255, 255, 255) : al_map_rgb(255, 0, 0);
    this->color = c;
}
