
#include "material.hpp"
#include "global.hpp"

Material::Material() : diffuse(), specular(), emissive(), ambient(), shine() {
    this->setAmbient(defaultMatAmbient);
    this->setDiffuse(defaultMatDiffuse);
    this->setSpecular(defaultMatSpecular);
    this->setEmissive(defaultMatEmissive);
    this->setShine(defaultMatShine);
}

void Material::setDiffuse(const float *diffuse_) {
    for (int i = 0; i < 4; ++i)
        diffuse[i] = diffuse_[i];
}

void Material::setAmbient(const float *ambient_) {
    for (int i = 0; i < 4; ++i)
        ambient[i] = ambient_[i];
}

void Material::setSpecular(const float *specular_) {
    for (int i = 0; i < 4; ++i)
        specular[i] = specular_[i];
    specular[3] = 1;
}

void Material::setEmissive(const float *emissive_) {
    for (int i = 0; i < 4; ++i)
        emissive[i] = emissive_[i];
    emissive[3] = 1;
}

void Material::setShine(float shine_) {
    this->shine = shine_;
}

void Material::setAmbientToDiffuse() {
    this->setAmbient(diffuse);
}

void Material::setDiffuseToAmbient() {
    this->setDiffuse(ambient);
}


void Material::apply() {
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
}

bool Material::operator==(const Material &rhs) const {
    bool res = true;
    for (int i = 0; i < 4 && res; i++) {
        res = res &&
              (ambient[i] == rhs.ambient[i]) &&
              (diffuse[i] == rhs.diffuse[i]) &&
              (specular[i] == rhs.specular[i]) &&
              (emissive[i] == rhs.emissive[i]);
    }
    res = res && shine == rhs.shine;
    return res;
}

bool Material::operator!=(const Material &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Material &material) {
    os << "ambient: " << material.ambient[0] << " " << material.ambient[1] << " " << material.ambient[2] << " "
       << material.ambient[3]
       << "\ndiffuse: " << material.diffuse[0] << " " << material.diffuse[1] << " " << material.diffuse[2] << " "
       << material.diffuse[3]
       << " \nspecular: " << material.specular[0] << " " << material.specular[1] << " " << material.specular[2] << " "
       << material.specular[3]
       << "\nemissive: " << material.emissive[0] << " " << material.emissive[1] << " " << material.emissive[2] << " "
       << material.emissive[3]
       << "\nshine: " << material.shine;
    return os;
}

