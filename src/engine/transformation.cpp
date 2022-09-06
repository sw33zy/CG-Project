#define _USE_MATH_DEFINES

#include <cmath>
#include <transformation.hpp>
#include "global.hpp"
#include "display.hpp"

// Superclass Transformation

float Transformation::elapsed = 0;
float Transformation::delay = 0;

void Transformation::updateDelay() {
    delay = (float) glutGet(GLUT_ELAPSED_TIME);
    delay -= elapsed;
}

void Transformation::tick() {
    Transformation::elapsed = (float) glutGet(GLUT_ELAPSED_TIME) - delay;
}

// Rotation

Rotation::Rotation(float x_, float y_, float z_, float angle_)
        :
        x(x_),
        y(y_),
        z(z_),
        angle(angle_) {}

void Rotation::apply() {
    glRotatef(angle, x, y, z);
}

// Scale


Scale::Scale(float x_, float y_, float z_)
        :
        x(x_),
        y(y_),
        z(z_) {}

void Scale::apply() {
    glScalef(x, y, z);
}

// Translation

Translation::Translation(float x_, float y_, float z_)
        :
        x(x_),
        y(y_),
        z(z_) {}


void Translation::apply() {
    glTranslatef(x, y, z);
}

// CatmullRoom Route

bool CatmullRomRoute::visuals = false;

CatmullRomRoute::CatmullRomRoute(float time, const std::vector<SimplePoint> &route)
        : time(time),
          data(route),
          face_forward(false) {}

CatmullRomRoute::CatmullRomRoute(float time, const std::vector<SimplePoint> &route, bool faceForward)
        : time(time),
          data(route),
          face_forward(faceForward) {}


void CatmullRomRoute::apply() {
    if (visuals)
        this->draw();

    float t = data.getLocalT(fmodf(elapsed, time) / time);
    SimplePoint pos = data.getPoint(t);

    glTranslatef(pos.x, pos.y, pos.z);

    if (face_forward) {
        Vector x = data.getTangent(t);
        Vector z = x.cross({0.0f, 1.0f, 0.0f});
        Vector y = z.cross(x);

        x.normalize();
        y.normalize();
        z.normalize();

        Matrix::matrix m;

        Matrix::rootMatrix(x, y, z, m);
        glMultMatrixf(reinterpret_cast<const float *>(m));

    }
}


void CatmullRomRoute::draw() {

    const int n = 100;
    const float factor = 1.0f / n;

    float mat[4];

    glPushAttrib(GL_LIGHTING_BIT);
    glGetMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
    for (int i = 0; i < 3; ++i) {
        mat[i] /= 2;
    }
    glMaterialfv(GL_FRONT, GL_EMISSION, mat);

    glBegin(GL_LINE_LOOP);


    for (int i = 0; i < n; i++) {
        SimplePoint p = data.getPoint(data.getLocalT((float) i * factor));
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    glPopAttrib();
}

//Spin

Spin::Spin(float time_, float axisX, float axisY, float axisZ)
        : time(time_),
          ax(axisX),
          ay(axisY),
          az(axisZ) {}

void Spin::apply() {
    float angle = (Transformation::elapsed * 360) / time;
    glRotatef(angle, ax, ay, az);
}

//Color

Color::Color(float red_, float green_, float blue_)
        :
        red(red_),
        green(green_),
        blue(blue_) {}

void Color::apply() {
    glColor3d(red, green, blue); //OLD WAY
}

