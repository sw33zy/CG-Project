#include <iostream>
#include <skybox.hpp>
#include "scenes.hpp"
#include "global.hpp"
#include "display.hpp"

//Group declarations

Group::Group()
        :
        models(),
        groups(),
        transformations(),
        materials() {}

void Group::draw(Model *obj[], Texture *texs[], Material *mats[]) {
    glPushMatrix();

    mats[0]->apply();
    for (Transformation *t : transformations)
        t->apply();
    for (unsigned int i = 0; i < models.size(); i++) {
        int index = textures[i];
        mats[materials[i]]->apply();
        if (index >= 0)
            obj[models[i]]->drawWithTexture(texs[index]);
        else
            obj[models[i]]->draw();
    }
    for (Group gp : this->groups)
        gp.draw(obj, texs, mats);

    glPopMatrix();
}

bool Scene::showRoutes = false;
bool Scene::animation = true;

bool Scene::toggleRoutes() {
    showRoutes = !showRoutes;
    CatmullRomRoute::visuals = showRoutes;
    return showRoutes;
};


Scene::Scene()
        :
        models(),
        instructions(),
        lights(),
        textures(),
        materials(),
        skybox_(nullptr) {
    materials.emplace_back(new Material());
}

Scene::~Scene() {
    this->disassemble();
}

void Scene::skybox(bool invertedCulling, const string &model, string *texFiles) {

    auto *m = new StripedModel(model, true);
    models.emplace_back(m);
    std::vector<Texture *> texs;

    int t;
    for (int i = 0; i < 6; i++) {
        t = identifyTexture(texFiles[i]);
        texs.emplace_back(textures[t]);
    }

    skybox_ = new Skybox(invertedCulling, m, texs.data());
}

int Scene::identifyTexture(const string &texture) {
    if (!texture.empty()) {
        for (unsigned int i = 0; i < textures.size(); i++) {
            if (textures[i]->getTextureFile() == texture)
                return i;
        }

        auto *res = new Texture(texture);
        textures.emplace_back(res);
        return (int) (textures.size() - 1);
    } else
        return -1;
}


ModelIndex Scene::identifyModel(const string &name, const string &description, bool isTextured) {
    for (ModelIndex i = 0; i < models.size(); i++) {
        if (models[i]->getFilename() == name)
            return i;
    }

    auto *m = new StripedModel(name, isTextured);
    if (!description.empty())
        m->setDescription(description);
    models.emplace_back(m);
    return models.size() - 1;
}

void Scene::disassemble() {
    for (Model *model : models)
        model->drop();

    for (Texture *t : textures)
        t->drop();
}

void Scene::assemble() {
    CatmullRomRoute::visuals = showRoutes;

    for (Light *l : lights) {
        l->prepare();
    }
    glEnable(GL_TEXTURE_2D);
    for (Texture *t : textures)
        t->load();

    for (Model *model : models)
        model->load();
}

void Scene::reassemble() {
    this->disassemble();
    this->assemble();
}

void Scene::draw() {

    glEnable(GL_TEXTURE_2D);
    if (skybox_) {
        glPushMatrix();
        glTranslatef(px, py, pz);
        skybox_->draw();
        glPopMatrix();
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    for (Light *l : lights)
        if (l) l->render();

    if (animation)
        Transformation::tick();
    instructions.draw(models.data(), textures.data(), materials.data());

    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_TEXTURE_2D);

}

void Scene::setInstructions(const Group &instructions_) {
    Scene::instructions = instructions_;
}

void Scene::insertLight(const SimplePoint &point, bool punctual, LightParameters pars) {
    int res = (int) lights.size();
    if (res < 9)
        lights.emplace_back(new Light(res, point, punctual, pars));
}

unsigned int Scene::identifyMaterial(Material m) {
    for (unsigned int i = 0; i < materials.size(); i++) {
        if (*(materials[i]) == m)
            return i;
    }
    materials.emplace_back(new Material(m));
    return materials.size() - 1;
}

bool Scene::toggleAnimation() {
    if (!animation)
        Transformation::updateDelay();

    return Scene::animation = !Scene::animation;
}




