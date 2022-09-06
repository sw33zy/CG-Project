#include <cmath>
#include <string>
#include <sstream>
#include "display.hpp"
#include "global.hpp"

int FramesPerSecond::timebase = 0;
float FramesPerSecond::frame = 0;

void FramesPerSecond::init() {
    frame = 0.0f;
    timebase = glutGet(GLUT_ELAPSED_TIME);
}

void FramesPerSecond::update() {
    frame++;

    int time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        float fps = (frame * 1000.0f) / (float) (time - timebase);
        timebase = time;
        frame = 0;

        std::string st("Grupo 19 - FPS: ");
        st.append(std::to_string(fps));
        glutSetWindowTitle(st.c_str());
    }
}


float beta = 0.5, alpha = 0, r = 150;
float px = 0;
float py = r * sin(beta);
float pz = r * cos(beta);


void update_cords() {
    px = r * cos(beta) * sin(alpha);
    py = r * sin(beta);
    pz = r * cos(beta) * cos(alpha);
}

void changeSize(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = (float) w * 1.0f / (float) h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(90.0f, ratio, 1.0f, 1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void set_camara() {
    glLoadIdentity();
    gluLookAt(px, py, pz, 0, 0, 0, 0, 1, 0);
}

// Camara Callback registry

bool camera_keyboard_handler(unsigned char key, int x, int y) {
    switch (key) {
        case 'h':
        case 'H':
            beta = 0.5;
            alpha = 0;
            r = 5;
            break;
        case 'w':
        case 'W':
            if (beta < 1.5) {
                beta += 0.1;
            } else beta = -1.5;
            break;

        case 's':
        case 'S':
            if (beta > -1.5) {
                beta -= 0.1;
            } else beta = 1.5;
            break;

        case 'A':
        case 'a':
            alpha -= 0.1;
            break;
        case 'D':
        case 'd':
            alpha += 0.1;
            break;
        default:
            return false;
    }
    update_cords();
    return true;
}

bool camara_special_keys_handler(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            r -= 1;
            update_cords();
            break;
        case GLUT_KEY_DOWN:
            r += 1;
            update_cords();
            break;
        default:
            return false;
    }
    return true;
}

void camara_mouseButton(int button, int state, int xx, int yy) {
}

void camara_mouseMove(int x, int y) {
}
