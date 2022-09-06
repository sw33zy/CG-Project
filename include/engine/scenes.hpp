#ifndef SCENES_HPP
#define SCENES_HPP

#include <vector>
#include "global.hpp"
#include "models.hpp"
#include "transformation.hpp"
#include "light.hpp"

using std::vector;

class Group {
public:
    vector<ModelIndex> models;
    vector<int> textures;
    vector<unsigned int> materials;
    vector<Group> groups;
    vector<Transformation *> transformations;


    Group();

    void draw(Model *obj[],Texture * texs[], Material * mats[]);
};

class Skybox;

class Scene {
private:
    Skybox * skybox_;
    Group instructions;
    vector<Model *> models;
    vector<Light *> lights;
    vector<Texture *> textures;
    vector<Material *> materials;

public:
    static bool showRoutes,animation;
    static bool toggleRoutes();
    static bool toggleAnimation();

    Scene();

    ~Scene();

    void setInstructions(const Group &instructions_);

    ModelIndex identifyModel(const string &name, const string &description,bool isTextured);

    void assemble();

    void disassemble();

    void reassemble();

    void draw();

    void insertLight(const SimplePoint &point, bool b, LightParameters material);

    int identifyTexture(const string &texture);

    unsigned int identifyMaterial(Material m);

    void skybox(bool invertedCulling, const string & model, string *texFiles);
};

#endif // SCENES_HPP