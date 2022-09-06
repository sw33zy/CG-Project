#ifndef MODELS_HPP
#define MODELS_HPP

#include "utils/point.hpp"
#include "transformation.hpp"
#include "global.hpp"
#include "texture.hpp"
#include <utility>
#include <vector>
#include "material.hpp"
//Model Class

typedef unsigned int ModelIndex;

class Model {
private:
    string filename;

public:
    const string &getFilename() const;

    void setFilename(const string &filename);

    /**
     * Loads the Model into a Vertex Buffer Object
     *
     * @param VBO The Vertex Buffer Object.
     */
    virtual void load() {}

    virtual void drop() {};

    /**
     * Draws the model
     */
    virtual void draw() {};

    virtual void drawWithTexture(Texture * tex){draw();};

};


class BulkModel : public Model {
private:
    GLuint VBO = -1;
    int count;
public:
    explicit BulkModel(const string &filename);

    void load() override;

    void draw() override;

    void drop() override;
};


class IndexedModel : public Model {
private:
    GLuint VBO = -1, VBOindex = -1;
    int count;
public:
    explicit IndexedModel(const string &filename);

    void load() override;

    void draw() override;

    void drop() override;
};


class StripedModel : public Model {
private:
    string description;
    bool textured;

    GLuint VBO = -1, VBOindex = -1, VBOnormal = -1, VBOTextureCoord = -1;
    std::vector<int> count;

public:

    const std::vector<int> &getCount() const;

    StripedModel(const string &filename, bool textured_);

    void setDescription(const string &description);

    void load() override;

    void draw() override;

    void drop() override;

    void drawWithTexture(Texture *tex) override;

    void beginDraw() const;

    void endDraw() const;
};


#endif // MODELS_HPP