#include <cstdlib>
#include <cmath>

#include <iostream>
#include <functional>
#include"utils/tinyxml2.h"
#include"xmlparser.hpp"
#include"models.hpp"

using namespace tinyxml2;
using namespace std;
using std::function;

// Declarations

class BuilderInfo {
public:
    Scene *father;

    explicit BuilderInfo() : father() {}
};

Transformation *parseTransformation(const string &name, XMLElement *elem);

void parseModels(Group *gp, XMLNode *pNode, const BuilderInfo &info);

Group parseGroup(XMLNode *elem, const BuilderInfo &info);

Group parseXML(const string &file, const BuilderInfo &info);

// Implementations

SimplePoint parseXYZ(XMLElement *elem) {
    const char *t0;
    float x = ((t0 = elem->Attribute("x")) ? strtof(t0, nullptr) : 0.0f); // Default 0
    float y = ((t0 = elem->Attribute("y")) ? strtof(t0, nullptr) : 0.0f); // Default 0
    float z = ((t0 = elem->Attribute("z")) ? strtof(t0, nullptr) : 0.0f); // Default 0
    return {x, y, z};
}

Transformation *parseTranslation(XMLElement *elem) {
    Transformation *res;
    const char *time = elem->Attribute("time");
    if (time) {
        const char *faceForward = elem->Attribute("fwd");
        vector<SimplePoint> route;
        XMLElement *ap = elem->FirstChildElement("point");
        for (; ap; ap = ap->NextSiblingElement("point"))
            route.emplace_back(parseXYZ(ap));
        res = new CatmullRomRoute(strtof(time, nullptr) * 1000.0f, route,
                                  faceForward != nullptr && string(faceForward) == "true"
        );

    } else {
        SimplePoint coords = parseXYZ(elem);
        res = new Translation(coords.x, coords.y, coords.z);
    }
    return res;
}

void parseAmbDiffSpec(XMLElement *elem, vec4 *amb, vec4 *diff, vec4 *spec) {
    const char *ch;
    ch = elem->Attribute("ambR");
    if (ch) (*amb)[0] = strtof(ch, nullptr);
    ch = elem->Attribute("ambG");
    if (ch) (*amb)[1] = strtof(ch, nullptr);
    ch = elem->Attribute("ambB");
    if (ch) (*amb)[2] = strtof(ch, nullptr);

    ch = elem->Attribute("diffR");
    if (ch) (*diff)[0] = strtof(ch, nullptr);
    ch = elem->Attribute("diffG");
    if (ch) (*diff)[1] = strtof(ch, nullptr);
    ch = elem->Attribute("diffB");
    if (ch) (*diff)[2] = strtof(ch, nullptr);

    ch = elem->Attribute("specR");
    if (ch) (*spec)[0] = strtof(ch, nullptr);
    ch = elem->Attribute("specG");
    if (ch) (*spec)[1] = strtof(ch, nullptr);
    ch = elem->Attribute("specB");
    if (ch) (*spec)[2] = strtof(ch, nullptr);

    ch = elem->Attribute("specular");
    if (ch && !strcmp(ch, "ambient"))
        setVec4(*amb, *spec);

    ch = elem->Attribute("ambient");
    if (ch && !strcmp(ch, "specular"))
        setVec4(*spec, *amb);

}


Material parseMaterial(XMLElement *elem) {
    Material res;
    const char *ch;
    ch = elem->Attribute("shine");
    if (ch) res.shine = strtof(ch, nullptr);

    ch = elem->Attribute("emissG");
    if (ch) res.emissive[0] = strtof(ch, nullptr);
    ch = elem->Attribute("emissB");
    if (ch) res.emissive[1] = strtof(ch, nullptr);
    ch = elem->Attribute("emissR");
    if (ch) res.emissive[2] = strtof(ch, nullptr);

    parseAmbDiffSpec(elem, &res.ambient, &res.diffuse, &res.specular);

    return res;
}


Transformation *parseRotation(XMLElement *elem) {
    Transformation *res;
    const char *time = elem->Attribute("time");
    const char *t0;
    if (time) {
        float ax = ((t0 = elem->Attribute("axisX")) ? strtof(t0, nullptr) : 0.0f);
        float ay = ((t0 = elem->Attribute("axisY")) ? strtof(t0, nullptr) : 0.0f);
        float az = ((t0 = elem->Attribute("axisZ")) ? strtof(t0, nullptr) : 0.0f);
        res = new Spin(strtof(time, nullptr) * 1000.0f, ax, ay, az);
    } else {
        SimplePoint coords = parseXYZ(elem);
        float angle = ((t0 = elem->Attribute("angle")) ? strtof(t0, nullptr) : 0.0f); // Default 0
        res = new Rotation(coords.x, coords.y, coords.z, angle);
    }
    return res;
}

Transformation *parseTransformation(const string &name, XMLElement *elem) {
    Transformation *res = nullptr;

    if (name == "color") {
        const char *t0;
        float a, b, c;
        a = ((t0 = elem->Attribute("r")) ? strtof(t0, nullptr) : 0.0f);
        b = ((t0 = elem->Attribute("g")) ? strtof(t0, nullptr) : 0.0f);
        c = ((t0 = elem->Attribute("b")) ? strtof(t0, nullptr) : 0.0f);
        res = new Color(a, b, c);
    } else if (name == "translate") {
        res = parseTranslation(elem);
    } else if (name == "rotate") {
        res = parseRotation(elem);
    } else if (name == "scale") {
        SimplePoint t = parseXYZ(elem);
        res = new Scale(t.x, t.y, t.z);
    }
    return res;
}

void parseModels(Group *gp, XMLNode *pNode, const BuilderInfo &info) {

    for (XMLElement *ap = pNode->FirstChildElement("model"); ap; ap = ap->NextSiblingElement("model")) {
        const char
                *file = ap->Attribute("file"),
                *desc = ap->Attribute("description"),
                *tex = ap->Attribute("texture");


        string f = (file ? file : ""), d = (desc ? desc : ""), t = (tex ? tex : "");

        int it = info.father->identifyTexture(t);
        unsigned int mat = info.father->identifyMaterial(parseMaterial(ap));

        gp->textures.emplace_back(it);
        gp->materials.emplace_back(mat);
        gp->models.emplace_back(info.father->identifyModel(f, d, it >= 0));
    }
}

Group parseGroup(XMLNode *elem, const BuilderInfo &info) {
    Group gp;
    bool endTransformations = false;
    for (XMLNode *node = elem->FirstChildElement(); node; node = node->NextSibling()) {
        string name = string(node->ToElement()->Name());
        if (!endTransformations) {
            Transformation *t;
            if ((t = parseTransformation(name, node->ToElement()))) {
                gp.transformations.emplace_back(t);
            } else
                endTransformations = true;
        }
        if (endTransformations) {
            if (name == "models")
                parseModels(&gp, node, info);
            else if (name == "group") {
                gp.groups.emplace_back(parseGroup(node, info));
            } else if (name == "input") {
                gp.groups.emplace_back(parseXML(string(node->ToElement()->Attribute("file")), info));

            }
        }
    }
    return gp;
}

LightParameters parseLightParameters(XMLElement *elem) {
    LightParameters res;
    const char *ch;

    parseAmbDiffSpec(elem, &res.ambient, &res.diffuse, &res.specular);

    ch = elem->Attribute("spotDX");
    if (ch) res.spotDirection[0] = strtof(ch, nullptr);
    ch = elem->Attribute("spotDY");
    if (ch) res.spotDirection[1] = strtof(ch, nullptr);
    ch = elem->Attribute("specDZ");
    if (ch) res.spotDirection[2] = strtof(ch, nullptr);

    ch = elem->Attribute("spotExp");
    if (ch) res.spotExponent = strtof(ch, nullptr);

    ch = elem->Attribute("spotCutOff");
    if (ch) res.spotExponent = strtof(ch, nullptr);

    ch = elem->Attribute("attenuation");
    if (ch) {
        res.setAttenuationType(ch);
        ch = elem->Attribute("attFX");
        if (ch) res.attenuationFactor[0] = strtof(ch, nullptr);
        ch = elem->Attribute("attFY");
        if (ch) res.attenuationFactor[1] = strtof(ch, nullptr);
        ch = elem->Attribute("attFZ");
        if (ch) res.attenuationFactor[2] = strtof(ch, nullptr);
    }
    return res;
}

void parseLights(XMLNode *pNode, const BuilderInfo &info) {
    if (!pNode)
        return;
    for (XMLElement *ap = pNode->FirstChildElement("light"); ap; ap = ap->NextSiblingElement("light")) {
        info.father->insertLight(parseXYZ(ap), !strcmp(ap->Attribute("type"), "POINT"), parseLightParameters(ap));
    }
}

void parseSkyBox(XMLElement *elem, const BuilderInfo &info) {
    if (!elem)
        return;
    bool inverted = false;

    const char *ch;

    ch = elem->Attribute("culling");
    if (ch) inverted = !strcmp(ch,"inverted");

    string model = elem->Attribute("model");
    std::vector<string> texs;
    string sh;

    sh = elem->Attribute("top");
    texs.emplace_back(sh);
    sh = elem->Attribute("bottom");
    texs.emplace_back(sh);
    sh = elem->Attribute("left");
    texs.emplace_back(sh);
    sh = elem->Attribute("right");
    texs.emplace_back(sh);
    sh = elem->Attribute("front");
    texs.emplace_back(sh);
    sh = elem->Attribute("back");
    texs.emplace_back(sh);


    info.father->skybox(inverted, model, texs.data());
}

Group parseXML(const string &file, const BuilderInfo &info) {
    tinyxml2::XMLDocument doc;
    Group main;

    XMLError error;
    if ((error = doc.LoadFile(file.c_str())) == XML_SUCCESS) {

        XMLNode *ap = doc.FirstChildElement("scene");
        parseLights(ap->FirstChildElement("lights"), info);
        parseSkyBox(ap->FirstChildElement("skybox"), info);

        ap = ap->FirstChildElement("group");
        for (; ap; ap = ap->NextSiblingElement("group")) {
            main.groups.emplace_back(parseGroup(ap, info));
        }

    } else
        std::cerr << "Error Opening XML\n\tCode " << error << " : "
                  << tinyxml2::XMLDocument::ErrorIDToName(error) << std::endl;

    return main;
}


Scene *parseScene(const string &file) {
    auto *res = new Scene();


    BuilderInfo info;

    info.father = res;

    res->setInstructions(parseXML(file, info));

    return res;
}
