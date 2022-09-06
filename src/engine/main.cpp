#include <fstream>
#include "global.hpp"
#include <iostream>
#include "display.hpp"
#include "xmlparser.hpp"

using namespace std;

Scene *mainScene = nullptr;

void init(int argc, char **argv);

void renderScene();

//User Event Handlers
void keyboard_event_handler(unsigned char key, int x, int y) {
    if (!camera_keyboard_handler(key, x, y)) {
        if (key == ' ')
            Scene::toggleAnimation();
        else if (key == 'p')
            Scene::toggleRoutes();
    }
    glutPostRedisplay();
}

void special_keys_handler(int key, int px, int py) {
    if (!camara_special_keys_handler(key, px, py)) {

    }
    glutPostRedisplay();

}

void mouseButton(int button, int state, int xx, int yy) {
}

void mouseMove(int x, int y) {
}


int main(int argc, char **argv) {
    //Init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1366, 768);
    glutCreateWindow("Grupo 19");

    //Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    //Keyboard callbacks
    glutKeyboardFunc(keyboard_event_handler);
    glutSpecialFunc(special_keys_handler);

    //Mouse callbacks
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    //Init Glew
#ifndef __APPLE__
    glewInit();
#endif

    //Init IL
    ilInit();

    init(argc, argv);

    // enter GLUT's main cycle
    glutMainLoop();

    return 0;
}

void init(int argc, char **argv) {
    FramesPerSecond::init();
    if (argc > 1) {
        mainScene = parseScene(string(argv[1]));
        for (int i = 2; i < argc; i++) {
            string val = string(argv[i]);
            if (val == "-paths")
                Scene::showRoutes = true;
        }
    } else
        cout << "Missing input file" << endl;

    if (mainScene)
        mainScene->assemble();
}

void renderScene() {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set the camera
    set_camara();
    //Drawing instructions;
    mainScene->draw();
    //End of frame
    glutSwapBuffers();
    glFlush();
    FramesPerSecond::update();
    if (Scene::animation)
        glutPostRedisplay();
}

