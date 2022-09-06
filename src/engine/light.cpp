#include <utils/point.hpp>
#include "light.hpp"
#include "global.hpp"


GLenum lightenum(int i) {
    switch (i) {
        case 1:
            return GL_LIGHT1;
        case 2:
            return GL_LIGHT2;
        case 3:
            return GL_LIGHT3;
        case 4:
            return GL_LIGHT4;
        case 5:
            return GL_LIGHT5;
        case 6:
            return GL_LIGHT6;
        case 7:
            return GL_LIGHT7;
        default:
            return GL_LIGHT0;
    }
}


Light::Light(int id, SimplePoint pos_, bool punctual, const LightParameters &parameters_)
        : light(), pos(), parameters() {
    this->light = lightenum(id);
    parameters = parameters_;
    pos[0] = pos_.x;
    pos[1] = pos_.y;
    pos[2] = pos_.z;
    pos[3] = (punctual ? 1.0f : 0.0f);
}

void Light::render() {
    glLightfv(light, GL_POSITION, pos);
    //DEBUG STUFF
    //glPushMatrix();
    //glTranslatef(pos[0], pos[1], pos[2]);
    //glutWireCube(0.5);
    //glPopMatrix();
}

void Light::prepare() {
    glEnable(light);
    parameters.set(light);
}


void LightParameters::set(GLenum light) {
    glLightfv(light, GL_AMBIENT, ambient);
    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_SPECULAR, specular);

    glLightfv(light, GL_SPOT_DIRECTION, spotDirection);
    glLightfv(light, GL_SPOT_EXPONENT, &spotExponent);
    glLightfv(light, GL_SPOT_CUTOFF, &spotCutOff);

    if (attenuation != GL_INVALID_ENUM)
        glLightfv(light, attenuation, attenuationFactor);

}


LightParameters::LightParameters() : ambient(), diffuse(), specular(), spotDirection(),
                                     spotExponent(defaultSpotExponent), spotCutOff(defaultSpotCutOff),
                                     attenuation(GL_INVALID_ENUM), attenuationFactor() {
    setVec4(defaultLigAmbient, ambient);
    setVec4(defaultLigDiffuse, diffuse);
    setVec4(defaultLigSpecular, specular);
    setVec3(defaultSpotDirection, spotDirection);
    setVec3(defaultSpotDirection, attenuationFactor); //Its the same default vector
}


void LightParameters::setAttenuationType(const string &type) {
    if (type == "CONSTANT")
        attenuation = GL_CONSTANT_ATTENUATION;
    else if (type == "LINEAR")
        attenuation = GL_LINEAR_ATTENUATION;
    else if (type == "QUADRATIC")
        attenuation = GL_QUADRATIC_ATTENUATION;
}