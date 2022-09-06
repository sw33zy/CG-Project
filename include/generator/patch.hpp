#ifndef PATCH_HPP
#define PATCH_HPP

#include "primitive.hpp"

using std::vector;
using namespace Matrix;

class BezierPrimitive {
private:
    vector<int *> patches;
    vector<Point> points;

public:
    BezierPrimitive();

    ~BezierPrimitive();


    string toString();

    bool readBezierPatches(char *fileName);

    Primitive generateBezierPatches(int t);

    friend Point getBezierPoint(float u, float v, const matrix &X, const matrix &Y, const matrix &Z);

    Point fixBezierPoint(Point failure, int t, int iu, int iv, const matrix &Z, const matrix &X,
                         const matrix &Y, int patchIndex);

};


#endif // PATCH_HPP
