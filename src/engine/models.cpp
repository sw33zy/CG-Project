#include <fstream>
#include "global.hpp"
#include "models.hpp"
#include <sstream>
#include "material.hpp"

using std::ifstream;

//Getters and Setters

const string &Model::getFilename() const {
    return filename;
}

void Model::setFilename(const string &filename_) {
    filename = filename_;
}

//Bulk Model

BulkModel::BulkModel(const string &fileName)
        : count(0) {
    this->setFilename(fileName);
}

/**
 * Loads the Model into a Vertex Buffer Object
 *
 * @param VBO The Vertex Buffer Object.
 */
void BulkModel::load() {
    std::vector<float> arr;

    float x, y, z;
    ifstream file(this->getFilename());
    while (file >> x >> y >> z) {
        arr.emplace_back(x);
        arr.emplace_back(y);
        arr.emplace_back(z);
    }
    file.close();

    count = (int) arr.size();

    glEnableClientState(GL_VERTEX_ARRAY);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (int) (count * sizeof(float)), arr.data(), GL_STATIC_DRAW);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/**
 * Draws the model
 */
void BulkModel::draw() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexPointer(3, GL_FLOAT, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glDisableClientState(GL_VERTEX_ARRAY);

}


void BulkModel::drop() {
    glDeleteBuffers(1, &VBO);
}

//Indexed model

IndexedModel::IndexedModel(const string &fileName)
        : count(), VBOindex() {
    this->setFilename(fileName);
}

/**
 * Loads the Model into a Vertex Buffer Object
 *
 * @param VBO The Vertex Buffer Object.
 */
void IndexedModel::load() {

    std::vector<float> points;
    std::vector<int> indexes;
    string line;

    float np, ni;
    ifstream file(this->getFilename());

    file >> np >> ni;

    while (np-- > 0) {
        float x, y, z;
        file >> x >> y >> z;
        points.emplace_back(x);
        points.emplace_back(y);
        points.emplace_back(z);
    }

    while (ni-- > 0) {
        int i;
        file >> i;
        indexes.emplace_back(i);
    }
    file.close();

    count = (int) indexes.size();

    glEnableClientState(GL_VERTEX_ARRAY);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBOindex);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (int) (points.size() * sizeof(float)), points.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int) (count * sizeof(int)), indexes.data(), GL_STATIC_DRAW);

    glDisableClientState(GL_VERTEX_ARRAY);
}

/**
 * Draws the model
 */
void IndexedModel::draw() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexPointer(3, GL_FLOAT, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    glDisableClientState(GL_VERTEX_ARRAY);

}

void IndexedModel::drop() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBOindex);
}

//-------------------------------------------------------------------------------------


StripedModel::StripedModel(const string &filename, bool textured_)
        : VBO(), VBOindex(), VBOnormal(), count(), description(), textured(textured_) { this->setFilename(filename); }

void StripedModel::load() {
    std::vector<float> points;
    std::vector<int> indexes;
    std::vector<float> normals;
    std::vector<float> textures;


    string line;

    int np, ns;
    ifstream file(this->getFilename());


    getline(file, line);
    np = stoi(line);
    getline(file, line);
    ns = stoi(line);

    while (np-- > 0) {
        getline(file, line);
        std::stringstream ss(line);
        string value;
        //Point
        for (int j = 0; j < 3; ++j) {
            std::getline(ss, value, ' ');
            points.emplace_back(stof(value));
        }
        //Normal
        for (int j = 0; j < 3; ++j) {
            std::getline(ss, value, ' ');
            normals.emplace_back(stof(value));
        }
        //Texture
        for (int j = 0; j < 2; ++j) {
            std::getline(ss, value, ' ');
            textures.emplace_back(stof(value));
        }
    }
    unsigned int n = 0;
    while (ns-- > 0) {
        getline(file, line);
        std::stringstream ss(line);
        string value;
        unsigned int c;
        for (c = 0; std::getline(ss, value, ' '); c++) {
            indexes.emplace_back(stoi(value));
        }
        n += c;
        count.emplace_back(c);
    }

    file.close();

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBOnormal);
    glGenBuffers(1, &VBOindex);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (int) (points.size() * sizeof(float)), points.data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
    glBufferData(GL_ARRAY_BUFFER, (int) (normals.size() * sizeof(float)), normals.data(), GL_STATIC_DRAW);

    if (textured) {
        glGenBuffers(1, &VBOTextureCoord);
        glBindBuffer(GL_ARRAY_BUFFER, VBOTextureCoord);
        glBufferData(GL_ARRAY_BUFFER, (int) (textures.size() * sizeof(float)), textures.data(), GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int) (n * sizeof(int)), indexes.data(), GL_STATIC_DRAW);
}




void StripedModel::drop() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBOindex);
    glDeleteBuffers(1, &VBOnormal);
    glDeleteBuffers(1, &VBOTextureCoord);
}


void StripedModel::setDescription(const string &description_) {
    StripedModel::description = description_;
}

void StripedModel::draw() {
    this->drawWithTexture(nullptr);
}


void StripedModel::beginDraw() const{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexPointer(3, GL_FLOAT, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
    glNormalPointer(GL_FLOAT, 0, nullptr);

    if (textured) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, VBOTextureCoord);
        glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindex);
}


void StripedModel::endDraw() const{
    //Unbind Texture
    if (textured) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        Texture::unbind();
    }

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void StripedModel::drawWithTexture(Texture *tex) {

    beginDraw();

    if (textured && tex)
        tex->bind();

    unsigned int start = 0;
    for (int size : count) {
        glDrawElements(GL_TRIANGLE_STRIP, size, GL_UNSIGNED_INT, (int *) (sizeof(unsigned int) * start));
        start += size;
    }

    endDraw();
}

const std::vector<int> &StripedModel::getCount() const {
    return count;
}

