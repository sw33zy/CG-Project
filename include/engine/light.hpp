
#ifndef ASSIGNMENT_LIGHT_HPP
#define ASSIGNMENT_LIGHT_HPP

#include "global.hpp"

class LightParameters {
public:

    vec4 ambient, diffuse, specular;
    vec3 spotDirection,attenuationFactor;
    float spotExponent, spotCutOff;
    GLenum attenuation;

    LightParameters();

    void set(GLenum light);

    void setAttenuationType(const string &type);
};

class Light {
    GLenum light;
    vec4 pos;
    LightParameters parameters;

public:

    Light(int id, SimplePoint pos, bool punctual, const LightParameters &parameters_);

    void prepare();

    void render();

};

const vec4 defaultLigAmbient = {0.0f, 0.0f, 0.0f, 1.0f};
const vec4 defaultLigDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};
const vec4 defaultLigSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
const vec3 defaultSpotDirection = {0.0f, 0.0f, 1.0f};
const float defaultSpotExponent = 0.0f;
const float defaultSpotCutOff = 180.0f;


#endif //ASSIGNMENT_LIGHT_HPP
