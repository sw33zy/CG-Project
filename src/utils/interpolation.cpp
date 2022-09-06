#include "point.hpp"
#include "interpolation.hpp"
#include<vector>
#include <cmath>
#include <iostream>

using namespace Matrix;

CatmullRomCurve::CatmullRomCurve(const std::vector<SimplePoint>& points)
        : points(points) {
    if (points.size() < 4) {
        std::cout << "Route Error: At least 4 strips must be given for a route.\nThis invalid route will be ignored."
                  << std::endl;
        this->points.clear();
    }
}

void CatmullRomCurve::updateData(const SimplePoint &p0, const SimplePoint &p1, const SimplePoint &p2, const SimplePoint &p3) {
    Vector V1 = {p0.x, p1.x, p2.x, p3.x};
    Vector V2 = {p0.y, p1.y, p2.y, p3.y};
    Vector V3 = {p0.z, p1.z, p2.z, p3.z};

    v1 = mulMatrixVector(V1, CatmullRomM);
    v2 = mulMatrixVector(V2, CatmullRomM);
    v3 = mulMatrixVector(V3, CatmullRomM);
}

float CatmullRomCurve::getLocalT(float globalT) {
    int POINT_COUNT = (int) points.size();
    float t = globalT * (float) POINT_COUNT; // this is the real global t
    int index = (int) std::floor(t);  // which segment
    t = t - (float) index; // where within  the segment

    int ip0 = (index + POINT_COUNT - 1) % POINT_COUNT;
    //IF the current starting index is outdated, update the object's curve
    if (index0 != ip0) {
        index0 = ip0;
        int ip1 = (ip0 + 1) % POINT_COUNT;
        int ip2 = (ip1 + 1) % POINT_COUNT;
        int ip3 = (ip2 + 1) % POINT_COUNT;

        this->updateData(points[ip0], points[ip1], points[ip2], points[ip3]);
    }
    return t;
}


SimplePoint CatmullRomCurve::getPoint(float t) {
    Vector T = {t * t * t, t * t, t, 1};
    return {T.dot(v1), T.dot(v2), T.dot(v3)};
}

Vector CatmullRomCurve::getTangent(float t) {
    Vector T_der = {3 * t * t, 2 * t, 1};
    return {T_der.dot(v1), T_der.dot(v2), T_der.dot(v3)};
}
