#include <vector>
#include <fstream>
#include <sstream>
#include <utils/point.hpp>
#include <primitive.hpp>
#include <iostream>
#include "utils/interpolation.hpp"
#include "patch.hpp"

using std::vector;
using namespace Matrix;


//x(u, v) = U * M * Px * MB * V
//y(u, v) = U * M * Py * MB * V
//z(u, v) = U * M * Pz * MB * V
Point getBezierPoint(float u, float v, const matrix &X, const matrix &Y, const matrix &Z) {

    Vector Vector_u = {u * u * u, u * u, u, 1};
    Vector Vector_v = {v * v * v, v * v, v, 1};
    Vector Vector_ul = {3 * u * u, 2 * u, 1, 0};
    Vector Vector_vl = {3 * v * v, 2 * v, 1, 0};

    Vector MV = mulMatrixVector(Vector_v, BezierM);
    Vector MVl = mulMatrixVector(Vector_vl, BezierM);

    Vector MXMV = mulMatrixVector(mulMatrixVector(MV, X), BezierM);
    Vector MYMV = mulMatrixVector(mulMatrixVector(MV, Y), BezierM);
    Vector MZMV = mulMatrixVector(mulMatrixVector(MV, Z), BezierM);


    SimplePoint coords = {Vector_u.dot(MXMV), Vector_u.dot(MYMV), Vector_u.dot(MZMV)};

    Vector tangV =
            {
                    Vector_u.dot(mulMatrixVector(mulMatrixVector(MVl, X), BezierM)),
                    Vector_u.dot(mulMatrixVector(mulMatrixVector(MVl, Y), BezierM)),
                    Vector_u.dot(mulMatrixVector(mulMatrixVector(MVl, Z), BezierM))
            };

    Vector tangU =
            {Vector_ul.dot(MXMV), Vector_ul.dot(MYMV), Vector_ul.dot(MZMV)};


    Vector normal = tangV.cross(tangU);

    return {coords, normal.normalize()};
}

Point BezierPrimitive::fixBezierPoint(Point failure, int t, int iu, int iv, const matrix &X, const matrix &Y,
                                      const matrix &Z, int patchIndex) {
    Vector newNormal;
    Vector prevNv, prevNu, nextNu, nextNv;

    float nextV = ((float) (iv + 1)) / ((float) t);
    float v = ((float) iv) / ((float) t);
    float u = ((float) (iu)) / ((float) t);
    float nextU = ((float) (iu + 1)) / ((float) t);

    if (iu > 0) {
        prevNu = failure.getVector(getBezierPoint(u - 1 / (float) t, v, X, Y, Z));
        if (iv > 0) {
            prevNv = failure.getVector(getBezierPoint(u - 1 / (float) t, v - 1 / (float) t, X, Y, Z));
        }
    }
    if (iu < t) {
        nextNu = failure.getVector(getBezierPoint(nextU, v, X, Y, Z));
    };
    if (iv < t) {
        nextNv = failure.getVector(getBezierPoint(nextU, nextV, X, Y, Z));
    };


    if (iu == 0) {
        if (iv == 0)
            newNormal = nextNv.cross(nextNu);
        else if (iv == t)
            newNormal = nextNu.cross(prevNv); //Not Checked
        else {
            newNormal = nextNv.cross(nextNu);
            newNormal += prevNv.cross(nextNu);
        }
    } /*
    else if (iu == t) {
        if (iv == 0) TODO
            newNormal = nextNu.cross(prevNv);
        else if (iv == t)
            newNormal = prevNv.cross(nextNu);
        else {
            newNormal = nextNv.cross(nextNu);
            newNormal += prevNv.cross(nextNu);
        }
    } else {
        if (iv == 0) {
            newNormal = nextNu.cross(nextNv);
            newNormal += nextNv.cross(prevNu);
        } else if (iv == t) {
            newNormal = prevNu.cross(prevNv);
            newNormal += prevNv.cross(nextNu);
        } else {
            newNormal = prevNv.cross(nextNu);
            newNormal += nextNu.cross(nextNv);
            newNormal += nextNv.cross(prevNu);
            newNormal += prevNu.cross(prevNv);
        }
}*/

    if (!newNormal.isNan())
        failure.setNormal(newNormal.normalize());
    return failure;
}

Primitive BezierPrimitive::generateBezierPatches(int t) {
    const float TextureUnit = 1.0f / (float) t;

    Primitive prototype;
    matrix X, Y, Z;//, MXM, MYM, MZM;
    for (int patch = 0; patch < patches.size(); patch++) {
        int patchIndex = (t + 1) * (t + 1) * patch;

        //Calculate Matrix
        for (unsigned int w = 0; w < 4; w++) {
            for (unsigned int j = 0; j < 4; j++) {
                X[w][j] = points[patches[patch][w * 4 + j]].x;
                Y[w][j] = points[patches[patch][w * 4 + j]].y;
                Z[w][j] = points[patches[patch][w * 4 + j]].z;
            }
        }
        //getBezierMiddles(X, Y, Z, MXM, MYM, MZM);

        for (int i = 0; i <= t; i++) {
            float u = ((float) i) / ((float) t);
            if (i != t)
                prototype.newStrip();

            for (int j = 0; j <= t; j++) {
                float v = ((float) j) / ((float) t);

                //Calculate Indexes
                if (i != t) {
                    int ti0 = patchIndex + ((t + 1) * i) + j;
                    int ti1 = patchIndex + (t + 1) * (i + 1) + j;
                    prototype.pushIndexSegment(ti1, ti0);
                }
                //Calculate Point
                Point res = getBezierPoint(u, v, X, Y, Z);

                res.setTexture(u * TextureUnit, v * TextureUnit);

                //DEBUGING
                if (res.normal.isNan()) {
                    prototype.pushPoint(fixBezierPoint(res, t, i, j, X, Y, Z, patchIndex));
                } else
                    prototype.pushPoint(res);
            }
            //prototype.autoSnapStrip();
        }
    }
    return prototype;
}


bool BezierPrimitive::readBezierPatches(char *fileName) {
    std::fstream file;
    std::string line;

    file.open(fileName);

    if (file.is_open()) {
        getline(file, line);
        int numPatches = stoi(line);

        while (numPatches-- > 0) {
            getline(file, line);
            int *indexes = new int[16];
            std::stringstream ss(line);
            for (int j = 0; j < 16; ++j) {
                string value;
                std::getline(ss, value, ',');
                indexes[j] = stoi(value);
            }
            this->patches.emplace_back(indexes);
        }

        getline(file, line);
        int count = stoi(line);

        while (count-- > 0) {
            getline(file, line);
            string x, y, z;
            std::stringstream ss(line);
            std::getline(ss, x, ',');
            std::getline(ss, y, ',');
            std::getline(ss, z, ',');

            this->points.emplace_back(Point(stof(x), stof(y), stof(z)));
        }
        return true;
    }
    return false;
}

BezierPrimitive::BezierPrimitive() : patches(), points() {}

BezierPrimitive::~BezierPrimitive() =
default;

string BezierPrimitive::toString() {
    std::stringstream ss;
    ss << this->patches.size() << std::endl;
    for (auto &i : this->patches) {
        for (int j = 0; j < 15; j++)
            ss << i[j] << ", ";
        ss << i[15] << std::endl;
    }
    ss << this->points.size() << std::endl;
    for (auto &i : this->points)
        ss << i.getString(", ") << std::endl;

    return ss.str();
}

