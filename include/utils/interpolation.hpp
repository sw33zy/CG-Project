#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "point.hpp"
#include<vector>


const Matrix::matrix CatmullRomM = {{-0.5f, 1.5f,  -1.5f, 0.5f},
                                    {1.0f,  -2.5f, 2.0f,  -0.5f},
                                    {-0.5f, 0.0f,  0.5f,  0.0f},
                                    {0.0f,  1.0f,  0.0f,  0.0f}};
const Matrix::matrix BezierM = {{-1.0f, 3.0f,  -3.0f, 1.0f},
                                {3.0f,  -6.0f, 3.0f,  0.0f},
                                {-3.0f, 3.0f,  0.0f,  0.0f},
                                {1.0f,  0.0f,  0.0f,  0.0f}};

class CatmullRomCurve {
private:
    unsigned int index0{};
    Vector v1, v2, v3;

    void updateData(const SimplePoint &p0, const SimplePoint &p1, const SimplePoint &p2, const SimplePoint &p3);

public:
    std::vector<SimplePoint> points;

    explicit CatmullRomCurve(const std::vector<SimplePoint> &points);

    float getLocalT(float globalT);

    SimplePoint getPoint(float local_T);

    Vector getTangent(float local_T);
};

#endif // INTERPOLATION_H