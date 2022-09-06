#ifndef ASSIGNMENT_SKYBOX_HPP
#define ASSIGNMENT_SKYBOX_HPP

#include <vector>
#include "models.hpp"
#include "global.hpp"

class Skybox;

class Skybox {
    bool invertCull;

    StripedModel * model;
    Texture *textures[6];
public:

    explicit Skybox(bool invertCull,StripedModel * model,Texture *textures[6]);

    void draw();

};


#endif //ASSIGNMENT_SKYBOX_HPP
