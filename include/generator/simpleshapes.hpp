#ifndef SIMPLE_SHAPES_HPP
#define SIMPLE_SHAPES_HPP

#include <vector>
#include "utils/point.hpp"
#include "primitive.hpp"

namespace SimpleShapes {
    Primitive mkBox(float width, float length, float height, int divisions);

    Primitive mkCone(float height, float radius, int slices, int stacks);

    Primitive mkSphere(float radius, int slices, int stacks);

    Primitive mkPlane(float size);
}
#endif // SIMPLE_SHAPES_HPP