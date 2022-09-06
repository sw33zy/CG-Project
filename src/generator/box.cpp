#include "simpleshapes.hpp"


Primitive SimpleShapes::mkBox(float x, float z, float y, int divisions) {
    //Prevent divisions by 0
    if (divisions < 1) divisions = 1;
    Primitive prototype;
    float xUnit = x / (float) divisions;
    float yUnit = y / (float) divisions;
    float zUnit = z / (float) divisions;
    float tUnit = 1.0f / (float) divisions;
    float x2 = x / 2, y2 = y / 2, z2 = z / 2;

    IndexStrip sides[6];

    float
            tx = 1.0f,
            ix = -x2,
            iy = -y2,
            iz = -z2;

    for (int i = 1; i <= divisions; i++) {
        //Vars
        float
                tx_next =1.0f- (float) i * tUnit,
                i1x = xUnit * (float) i - x2,
                i1y = yUnit * (float) i - y2,
                i1z = zUnit * (float) i - z2;

        //New Strips
        for (auto &side : sides)
            side = IndexStrip();

        float ty = 1;
        for (int j = 0; j <= divisions; j++) {
            float ty_next = 1.0f-(float) (j+1) * tUnit;
            float jx = xUnit * (float) j - x2, jy = yUnit * (float) j - y2, jz = zUnit * (float) j - z2;
            //Top (xOz)
            Point p1 = {-jx, y2, i1z, 0.0f, 1.0f, 0.0f, tx_next,ty};
            Point p2 = {-jx, y2, iz, 0.0f, 1.0f, 0.0f, tx,  ty};
            sides[0].pushIndex(prototype.indexPoint(p1));
            sides[0].pushIndex(prototype.indexPoint(p2));

            //Bottom (-xOz)

            p1 = {jx, -y2, i1z, 0.0f, -1.0f, 0.0f,tx_next, ty};
            p2 = {jx, -y2, iz, 0.0f, -1.0f, 0.0f, tx, ty};
            sides[1].pushIndex(prototype.indexPoint(p1));
            sides[1].pushIndex(prototype.indexPoint(p2));

            //Left (xOy)

            p1 = {-jx, iy, z2, 0.0f, 0.0f, 1.0f,ty,tx};
            p2 = {-jx, i1y, z2, 0.0f, 0.0f, 1.0f,  ty,tx_next};
            sides[2].pushIndex(prototype.indexPoint(p1));
            sides[2].pushIndex(prototype.indexPoint(p2));


            //Right (-xOy)
            p1 = {jx, iy, -z2, 0.0f, 0.0f, -1.0f, ty, tx};
            p2 = {jx, i1y, -z2, 0.0f, 0.0f, -1.0f, ty, tx_next};
            sides[3].pushIndex(prototype.indexPoint(p1));
            sides[3].pushIndex(prototype.indexPoint(p2));


            //Front (yOz)
            p1 = {x2, jy, i1z, 1.0f, 0.0f, 0.0f, tx_next, ty};
            p2 = {x2, jy, iz, 1.0f, 0.0f, 0.0f, tx,ty};
            sides[4].pushIndex(prototype.indexPoint(p1));
            sides[4].pushIndex(prototype.indexPoint(p2));


            //Back (-yOz)

            p1 = {-x2, jy, -i1z, -1.0f, 0.0f, 0.0f, tx_next, ty};
            p2 = {-x2, jy, -iz, -1.0f, 0.0f, 0.0f,  tx, ty};
            sides[5].pushIndex(prototype.indexPoint(p1));
            sides[5].pushIndex(prototype.indexPoint(p2));

            ty = ty_next;
        }

        //PushStrips
        for (auto &side : sides)
            prototype.pushStrip(side);

        tx = tx_next;
        ix = i1x;
        iy = i1y;
        iz = i1z;
    }
    return prototype;
}