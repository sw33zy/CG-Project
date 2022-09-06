#ifndef POINT_H
#define POINT_H

#include <string>
#include <ostream>

using std::string;

typedef unsigned int Index;


class SimplePoint {

public:
    // Coordinates
    float x, y, z;

    SimplePoint();

    SimplePoint(float x, float y, float z);

    bool operator==(const SimplePoint &rhs) const;

    bool operator!=(const SimplePoint &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const SimplePoint &point);

    string getString(const string &) const;

};


class Vector {

public:
    float x, y, z, w;

    Vector();

    Vector(float x, float y, float z);

    Vector(float x, float y, float z, float w);

    float dot(Vector) const;

    Vector cross(Vector) const;

    float norm() const;

    float *array() const;

    Vector &normalize();

    string getString(const string &) const;


    bool operator==(const Vector &rhs) const;

    bool operator!=(const Vector &rhs) const;

    Vector& operator+=(const Vector& rhs);

    friend std::ostream &operator<<(std::ostream &os, const Vector &vector);

    friend Vector operator+(Vector const &c1, Vector const &c2);

    friend Vector operator-(Vector const &c1, Vector const &c2);

    bool isNan() const;

    bool isNull() const;
};


class Point {
public:
    float x, y, z, texture_x, texture_y;
    Vector normal;

    Point();

    Point(float x, float y, float z);

    Point(float x, float y, float z, float normalX, float normalY, float normalZ);

    Point(float x, float y, float z, float normalX, float normalY, float normalZ, float textureX,
          float textureY);

    Point(SimplePoint &point, Vector &normal);

    Point(SimplePoint &point, Vector &normal, float textureX, float textureY);

    SimplePoint getCoord() const;

    Vector getVector(Point p) const;

    void setTexture(float tx, float ty);

    string getString(const string &) const;

    bool operator==(const Point &rhs) const;

    bool operator!=(const Point &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Point &point);

    void setNormal(Vector vector);
};

namespace Matrix {
    typedef float matrix[4][4];

    void rootMatrix(Vector pt1, Vector pt2, Vector pt3, matrix output);

    void transpose(matrix m);

    void mulMatrix(const matrix &fst, const matrix &snd, matrix &output);

    Vector mulMatrixVector(const Vector &vector, matrix const &m);
};


#endif // POINT_H
