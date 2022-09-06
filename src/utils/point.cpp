#include "point.hpp"
#include <string>
#include <sstream>
#include <limits>

using std::string;

// SIMPLEPOINT --------------------------------------------------------------------------------------------------------

//----
SimplePoint::SimplePoint()
        : x(0), y(0), z(0) {}

SimplePoint::SimplePoint(float x, float y, float z)
        : x(x), y(y), z(z) {}

//----

string SimplePoint::getString(const string &regex) const {
    std::ostringstream sstream;
    sstream.precision(std::numeric_limits<double>::max_digits10);
    sstream << x << regex << y << regex << z;
    return sstream.str();
}

//----
bool SimplePoint::operator!=(const SimplePoint &rhs) const {
    return !(rhs == *this);
}

bool SimplePoint::operator==(const SimplePoint &rhs) const {
    return x == rhs.x &&
           y == rhs.y &&
           z == rhs.z;
}

std::ostream &operator<<(std::ostream &os, const SimplePoint &point) {
    os << "x: " << point.x << " y: " << point.y << " z: " << point.z;
    return os;
}

// POINT --------------------------------------------------------------------------------------------------------------

// Constructors ---------------------------------------------------------------------------------------------------
Point::Point(float x, float y, float z)
        : x(x), y(y), z(z), normal(),
          texture_x(0.0f), texture_y(0.0f) {}

Point::Point()
        : x(0.0f), y(0.0f), z(0.0f), normal(),
          texture_x(0.0f), texture_y(0.0f) {}

Point::Point(float x, float y, float z, float normalX, float normalY, float normalZ)
        : x(x), y(y), z(z),
          normal(normalX, normalY, normalZ),
          texture_x(0.0f), texture_y(0.0f) {
    normal.normalize();
}

Point::Point(float x, float y, float z, float normalX, float normalY, float normalZ, float textureX, float textureY)
        : x(x), y(y), z(z),
          normal(normalX, normalY, normalZ),
          texture_x(textureX), texture_y(textureY) {
    normal.normalize();
}

Point::Point(SimplePoint &point, Vector &normal_)
        : x(point.x), y(point.y), z(point.z),
          normal(normal_),
          texture_x(0.0f), texture_y(0.0f) {
    normal.normalize();
}

Point::Point(SimplePoint &point, Vector &normal_, float textureX, float textureY)
        : x(point.x), y(point.y), z(point.z),
          normal(normal_),
          texture_x(textureX), texture_y(textureY) {
    normal.normalize();
}

// Methods --------------------------------------------------------------------------------------------------------

void Point::setTexture(float tx, float ty) {
    this->texture_x = tx;
    this->texture_y = ty;
}

string Point::getString(const string &regex) const {
    std::ostringstream sstream;
    sstream.precision(std::numeric_limits<double>::max_digits10);
    sstream << x << regex << y << regex << z << regex << normal.getString(regex) << regex << texture_x << regex
            << texture_y;
    return sstream.str();
}

// Operators ------------------------------------------------------------------------------------------------------

bool Point::operator==(const Point &rhs) const {
    return x == rhs.x &&
           y == rhs.y &&
           z == rhs.z &&
           texture_x == rhs.texture_x &&
           texture_y == rhs.texture_y &&
           normal == rhs.normal;
}

bool Point::operator!=(const Point &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "x: " << point.x << " y: " << point.y << " z: " << point.z << " texture_x: " << point.texture_x
       << " texture_y: " << point.texture_y << " normal[" << point.normal << "]";
    return os;
}

void Point::setNormal(Vector vector) {
    normal = vector;
}

SimplePoint Point::getCoord() const {
    return {x,y,z};
}

Vector Point::getVector(Point p) const {
    return {p.x - x,p.y-y,p.z-z};
}



// VECTOR ------------------------------------------------------------------------------------------------------------

// Constructors ---------------------------------------------------------------------------------------------------

Vector::Vector()
        : x(0), y(0), z(0), w(0) {}

Vector::Vector(float x, float y, float z)
        : x(x), y(y), z(z), w(0.0f) {}

Vector::Vector(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) {}


// Methods --------------------------------------------------------------------------------------------------------

Vector Vector::cross(Vector in) const {
    return {
            this->y * in.z - this->z * in.y,
            this->z * in.x - this->x * in.z,
            this->x * in.y - this->y * in.x,
            0.0f
    };
}

float Vector::norm() const {
    return sqrt(x * x + y * y + z * z);
}

Vector &Vector::normalize() {
    float l = this->norm();
    x = x / l;
    y = y / l;
    z = z / l;
    w = 0.0f;
    return *this;
}

float Vector::dot(Vector input) const {
    return this->x * input.x + this->y * input.y + this->z * input.z + this->w * input.w;
}

float *Vector::array() const {
    return new float[4]{x, y, z, w};
}

string Vector::getString(const string &regex) const {
    std::ostringstream sstream;
    sstream.precision(std::numeric_limits<double>::max_digits10);
    sstream << x << regex << y << regex << z;
    return sstream.str();
}

// Operators ------------------------------------------------------------------------------------------------------


bool Vector::operator==(const Vector &rhs) const {
    return x == rhs.x &&
           y == rhs.y &&
           z == rhs.z &&
           w == rhs.w;
}

bool Vector::operator!=(const Vector &rhs) const {
    return !(rhs == *this);
}


Vector &Vector::operator+=(const Vector &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Vector &vector) {
    os << "x: " << vector.x << " y: " << vector.y << " z: " << vector.z << " w: " << vector.w;
    return os;
}

bool Vector::isNan() const {
    return _isnan(x) || _isnan(y) || _isnan(z) || _isnan(w);
}


Vector operator+(Vector const &c1, Vector const &c2) {
    return {c1.x + c2.x, c1.y + c2.y, c1.z + c2.z, c1.w + c2.w};
}

Vector operator-(Vector const &c1, Vector const &c2) {
    return {c1.x - c2.x, c1.y - c2.y, c1.z - c2.z, c1.w - c2.w};
}

bool Vector::isNull() const {
    return x==0 && y==0 && z == 0;
}



// MATRIX -------------------------------------------------------------------------------------------------------------


void Matrix::rootMatrix(Vector pt1, Vector pt2, Vector pt3, matrix output) {
    output[0][0] = pt1.x;
    output[0][1] = pt1.y;
    output[0][2] = pt1.z;

    output[1][0] = pt2.x;
    output[1][1] = pt2.y;
    output[1][2] = pt2.z;

    output[2][0] = pt3.x;
    output[2][1] = pt3.y;
    output[2][2] = pt3.z;

    output[0][3] = output[1][3] = output[2][3] = output[3][0] = output[3][1] = output[3][2] = 0;
    output[3][3] = 1;

}

void Matrix::transpose(matrix data) {
    for (int i = 0; i < 3; i++) {
        for (int j = i; j < 4; j++) {
            float t0 = data[i][j];
            data[i][j] = data[j][i];
            data[j][i] = t0;
        }
    }
}


void Matrix::mulMatrix(const matrix &fst, const matrix &snd, matrix &output) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++) {
                sum += fst[i][k] * snd[k][j];
            }
            output[i][j] = sum;
        }
    }
}

Vector Matrix::mulMatrixVector(const Vector &vector, matrix const &m) {
    return {
            vector.x * m[0][0] + vector.y * m[0][1] + vector.z * m[0][2] + vector.w * m[0][3],
            vector.x * m[1][0] + vector.y * m[1][1] + vector.z * m[1][2] + vector.w * m[1][3],
            vector.x * m[2][0] + vector.y * m[2][1] + vector.z * m[2][2] + vector.w * m[2][3],
            vector.x * m[3][0] + vector.y * m[3][1] + vector.z * m[3][2] + vector.w * m[3][3],
    };
}

/*
Vector Matrix::mulVectorMatrix(const Vector &vector, matrix const &m) {
    float res[4];

    float *v = vector.array();

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j][k];
        }
    }

    delete v;
    return {res[0], res[1], res[2], res[3]};
}

 */


