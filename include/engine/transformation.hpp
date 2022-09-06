#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <vector>
#include <utils/point.hpp>
#include <utils/interpolation.hpp>

class Transformation {
protected:
    static float elapsed;
    static float delay;

public:
    static void tick();

    static void updateDelay();

    virtual ~Transformation() = default;

    virtual void apply() = 0;

};

class Color : public Transformation {
private:
    float red, green, blue;

public:
    Color(float red, float green, float blue);

    void apply() override;
};

class Rotation : public Transformation {
private:
    float x, y, z, angle;

public:
    Rotation(float x, float y, float z, float angle);

    void apply() override;
};

class Scale : public Transformation {
private:
    float x, y, z;
public:
    Scale(float x, float y, float z);

    void apply() override;
};

class Translation : public Transformation {
private:
    float x, y, z;

public:
    Translation(float x, float y, float z);

    void apply() override;
};

class CatmullRomRoute : public Transformation {
private:

    float time;
    bool face_forward;
    CatmullRomCurve data;

public:
    CatmullRomRoute(float time, const std::vector<SimplePoint> &route);

    CatmullRomRoute(float time, const std::vector<SimplePoint> &route, bool faceForward);

    void apply() override;

    void draw();

    static bool visuals;
};

class Spin : public Transformation {
private:
    float ax, ay, az, time;
    static float elapsed;

public:


    Spin(float time_, float axisX, float axisY, float axisZ);

    void apply() override;

};


#endif // TRANSFORMATION_HPP