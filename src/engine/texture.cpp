#include "texture.hpp"

#include <utility>
#include <iostream>

void Texture::load() {
    unsigned int t, tw, th;
    unsigned char *texData;
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) textureFile.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int) tw, (int) th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    ilDeleteImages(1, &t);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const {
    if (id >= 0)
        glBindTexture(GL_TEXTURE_2D, id);

}

void Texture::drop() {
    glDeleteTextures(1, &id);
    id = -1;
}

const std::string &Texture::getTextureFile() const {
    return textureFile;
}

Texture::Texture(std::string textureFile) : textureFile(std::move(textureFile)), id(-1) {}

CubeMapTexture::CubeMapTexture(std::string texFiles[]) : texFiles_() {
    for (int i = 0; i < 6; i++)
        texFiles_[i] = texFiles[i];
}

void CubeMapTexture::bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void CubeMapTexture::load() {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    for (unsigned int i = 0; i < 6; i++) {
        unsigned int t, tw, th;
        ilGenImages(1, &t);
        ilBindImage(t);
        ilLoadImage((ILstring) texFiles_[i].c_str());
        tw = ilGetInteger(IL_IMAGE_WIDTH);
        th = ilGetInteger(IL_IMAGE_HEIGHT);
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        unsigned char *texData = ilGetData();

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X +i, 0, GL_RGBA, (int) tw, (int) th, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, texData);


        ilDeleteImages(1, &t);
    }




    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMapTexture::drop() {
    glDeleteTextures(1, &id);
}

