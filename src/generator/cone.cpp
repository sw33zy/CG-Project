#define _USE_MATH_DEFINES

#include<cmath>
#include "simpleshapes.hpp"
#include "indexed.hpp"


const float TextureRadius = 0.25f;
const float TextureBaseCenterX = 0.25f;
const float TextureBaseCenterY = 0.5f;
const float TextureSideCenterX = 0.75f;
const float TextureSideCenterY = 0.5f;


Primitive SimpleShapes::mkCone(float height, float radius, int slices, int stacks) {

    if (stacks < 1) stacks = 1;
    if (slices < 1) stacks = 1;

    Primitive prototype;

    float h2 = height / 2.0f;
    double yUnit = height / (float) stacks;
    double rUnit = radius / (float) stacks;
    float TextureUnit = TextureRadius / (float) stacks;
    double alphaUnit = (2 * M_PI) / (double) slices;


    //Beta
    double beta = atan(height / radius);
    double cosBeta = cos(beta), sinBeta = sin(beta);

    //DRAW BASE
    prototype.newStrip();
    prototype.pushPoint({0.0f, -h2, 0.0f, 0.0f, -1.0f, 0.0f, TextureBaseCenterX, TextureBaseCenterY});
    for (int piece = 0; piece <= slices; piece++) {
        double alpha = piece * alphaUnit;
        double sinAlpha = sin(alpha),
                cosAlpha = cos(alpha);

        prototype.pushPoint(
                Point(
                        (float) (radius * sinAlpha),
                        -h2,
                        (float) (radius * cosAlpha),
                        0.0f,
                        -1.0f,
                        0.0f,
                        (float) (TextureBaseCenterX + TextureRadius * sinAlpha),
                        (float) (TextureBaseCenterY + TextureRadius * cosAlpha)
                )
        );
        prototype.pushIndex(piece + 1);
        prototype.pushIndex(0);
    }

    //DRAW SIDE -------------------------------------------------------------
    //Starting alpha is 0
    double sinCurrAlpha = 0.0;
    double cosCurrAlpha = 1.0;
    for (int piece = 1; piece <= slices; piece++) {
        double sinNextAlpha, cosNextAlpha, nextAlpha = piece * alphaUnit;

        sinNextAlpha = sin(nextAlpha);
        cosNextAlpha = cos(nextAlpha);

        prototype.newStrip();
        //Compute stacks
        for (int layer = 0; layer <= stacks; layer++) {
            double currRadius = (stacks - layer) * rUnit;
            float y = (float) (layer * yUnit) - h2;

            float textureRadius = TextureUnit * (float) layer;

            // Points
            Point left = Point(
                    (float) (currRadius * sinCurrAlpha),
                    y,
                    (float) (currRadius * cosCurrAlpha),
                    (float) (cosBeta * sinCurrAlpha),
                    (float) sinBeta,
                    (float) (cosBeta * cosCurrAlpha),
                    (float) (TextureSideCenterX + textureRadius * sinCurrAlpha),
                    (float) (TextureSideCenterY + textureRadius * cosCurrAlpha));

            if (layer != stacks) {
                Point right = Point(
                        (float) (currRadius * sinNextAlpha),
                        y,
                        (float) (currRadius * cosNextAlpha),
                        (float) (cosBeta * sinNextAlpha),
                        (float) sinBeta,
                        (float) (cosBeta * cosNextAlpha),
                        (float) (TextureSideCenterX + textureRadius * sinNextAlpha),
                        (float) (TextureSideCenterY + textureRadius * cosNextAlpha));


                prototype.autoPushSegment(left, right);
            } else {
                //The top point is different for every strip because of it's normal (OpenGL is dumb)
                prototype.autoPushPoint(left);
            }

        }
        sinCurrAlpha = sinNextAlpha;
        cosCurrAlpha = cosNextAlpha;
    }

    return prototype;
}