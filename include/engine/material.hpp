
#ifndef ASSIGNMENT_MATERIAL_HPP
#define ASSIGNMENT_MATERIAL_HPP

#include <ostream>
#include "global.hpp"

class Material {
public:
    vec4 ambient, diffuse, specular, emissive;
    float shine;

    friend std::ostream &operator<<(std::ostream &os, const Material &material);

    Material();

    void setDiffuse(const float *diffuse);

    void setSpecular(const float *specular);

    void setEmissive(const float *emissive);

    void setAmbient(const float *ambient);

    void apply();

    void setShine(float shine);

    bool operator==(const Material &rhs) const;

    bool operator!=(const Material &rhs) const;

    void setDiffuseToAmbient();

    void setAmbientToDiffuse();
};

const vec4 defaultMatAmbient = {0.2f, 0.2f, 0.2f, 1.0f};
const vec4 defaultMatDiffuse = {0.8f, 0.8f, 0.8f, 1.0f};
const vec4 defaultMatSpecular = {0.0f, 0.0f, 0.0f, 1.0f};
const vec4 defaultMatEmissive = {0.0f, 0.0f, 0.0f, 1.0f};
const float defaultMatShine = 0.0f;

#endif //ASSIGNMENT_MATERIAL_HPP
