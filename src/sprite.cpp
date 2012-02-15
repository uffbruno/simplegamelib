#include "sprite.hpp"

sprite::sprite(): current_animation(0), animations() {
}

sprite::~sprite() {
    for (unsigned int i = 0; i < animations.size(); ++i) {
        delete animations[i];
    }
    animations.clear();
}

void sprite::add_animation(animation *anim) {
    if (anim == NULL) return;
    animations.push_back(anim);
}

void sprite::set_animation(const unsigned int id) {
    if (id < animations.size()) {
        current_animation = id;
    } else {
        current_animation = 0;
    }
}

void sprite::run_animation() {
    if (animations.empty()) return;
    
    animation *anim = animations.at(current_animation);
    if (anim != NULL) {
        anim->next();
    }
}

animation* sprite::get_animation() const {
    if (animations.empty()) return NULL;
    return animations.at(current_animation);
}
