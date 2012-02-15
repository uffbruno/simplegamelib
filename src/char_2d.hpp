#ifndef CHAR_2D_HPP
#define CHAR_2D_HPP

#include "character.hpp"

enum char2d_state {
    CHAR2D_STILL_LEFT,
    CHAR2D_STILL_RIGHT,
    CHAR2D_WALKING_LEFT,
    CHAR2D_WALKING_RIGHT,
    CHAR2D_JUMPING_LEFT,
    CHAR2D_JUMPING_RIGHT
};

typedef enum char2d_state char2d_state;

class char_2d: public character {
    public:
        char_2d(float x, float y, float sx, float sy, const std::string& directory);
        virtual ~char_2d();
        
        void set_gravity(const float gravity) { this->gravity_force = gravity; }
        void set_jump_force(const float jumping_force) { this->jumping_force = jumping_force; }
        void set_vertical_speed_limit(const float vertical_speed_limit)
            { this->vertical_speed_limit = vertical_speed_limit; }
        vector2d handle_collision(const game_object* other);
		void print();

    private:
        float gravity_force;
        float jumping_force;
        float vertical_speed_limit;
		
		std::string statenames[6];
        
        virtual unsigned int impl_update();
        virtual void impl_draw() const;
        
        char_2d(const char_2d& other);
        char_2d& operator=(const char_2d& other);
};

#endif //CHAR_2D_HPP