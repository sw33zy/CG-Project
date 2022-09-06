#include "simpleshapes.hpp"


Primitive SimpleShapes::mkPlane(float size) {
    Primitive prototype;

    Point p1 = {size, 0.0f, size, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    Point p2 = {-size, 0.0f, -size, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    Point p3 = {size, 0.0, -size, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    Point p4 = {-size, 0.0, size, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

    prototype.pushPoint(p1);
    prototype.pushPoint(p2);
    prototype.pushPoint(p3);
    prototype.pushPoint(p4);

    prototype.newStrip();
    prototype.pushIndex(3);
    prototype.pushIndex(0);
    prototype.pushIndex(1);
    prototype.pushIndex(2);


    return prototype;
}