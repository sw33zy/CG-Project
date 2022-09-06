#ifndef ASSIGNMENT_TEXTURE_HPP
#define ASSIGNMENT_TEXTURE_HPP

#include <string>
#include <vector>
#include "global.hpp"

class Texture {
private:
    GLuint id;
    std::string textureFile;
public:

    explicit Texture(std::string textureFile);

    const std::string &getTextureFile() const;

    static void unbind();

    void bind() const;

    void load();

    void drop();

};

class CubeMapTexture{
private:
    GLuint id{};
    std::string texFiles_[6];

public:

    explicit CubeMapTexture(std::string texFiles[]);

    void bind() const;

    void load();

    void drop();

};


#endif //ASSIGNMENT_TEXTURE_HPP
