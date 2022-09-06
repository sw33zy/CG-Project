#include <vector>
#include <iostream>
#include "skybox.hpp"

Skybox::Skybox(bool invertCull_, StripedModel *model_, Texture *textures_[6])
        : model(model_), textures(), invertCull(invertCull_) {
    for (int i = 0; i < 6; i++)
        textures[i] = textures_[i];

}

void Skybox::draw() {

    glEnable(GL_COLOR_MATERIAL);
    glColor3d(1, 1, 1);

    if (invertCull)
        glFrontFace(GL_CW);

    glDepthMask(GL_FALSE);

    model->beginDraw();

    std::vector<int> m = model->getCount();

    unsigned int start = 0;
    int i = 0;
    for (int size : m) {
        textures[i % 6]->bind();

        glDrawElements(GL_TRIANGLE_STRIP, size, GL_UNSIGNED_INT, (int *) (sizeof(unsigned int) * start));
        start += size;
        i++;
    }

    model->endDraw();

    glDepthMask(GL_TRUE);
    glPopAttrib();
    if (invertCull)
        glFrontFace(GL_CCW);

}



