#include "simpleshapes.hpp"
#include "primitive.hpp"

#define _USE_MATH_DEFINES
#include<math.h>
#include <iostream>


Primitive SimpleShapes::mkSphere(float radius, int slices, int stacks) {

    //Prevent divisions by  0, or non 3d objects
    if (slices < 1) slices = 1;
    if (stacks < 1) stacks = 1;

    Primitive prototype;
    double alphaUnit = (2 * M_PI) / slices;
    double betaUnit = M_PI / stacks;

    float textureXUnit = 1.0f / (float) slices, textureYUnit = 1.0f / (float) stacks;

    //const float texture_x = alpha * texture_x_shift;


    //Precalculations
    std::vector<double> sinsAlpha(slices + 1);
    std::vector<double> cossAlpha(slices + 1);
    std::vector<float> Texture_Xs(slices + 1);
    sinsAlpha[0] =sinsAlpha[slices] = 0.0; //ALPHA = 0 && ALPHA = (2 * M_PI)
    cossAlpha[0] =cossAlpha[slices] = 1.0; //ALPHA = 0 && ALPHA = (2 * M_PI)
    Texture_Xs[0] = 0.0f;
    Texture_Xs[slices] = 1.0f;
    for (int i = 1; i < slices; i++) {
        double alpha = i * alphaUnit;
        sinsAlpha[i] = sin(alpha);
        cossAlpha[i] = cos(alpha);
        Texture_Xs[i] = (float) i * textureXUnit;
    }

    //Starting Beta is -M_PI_2
    float currY = -radius, currTY = 1.0f;  // radius * sin(-M_PI_2) = - radius
    double cathetus = 0.0, cosCurrBeta = 1.0, sinCurrBeta = 0.0f;   // radius * cos(-M_PI_2) = 0
    for (int layer = 1; layer <= stacks; layer++) {
        double nextBeta = layer * betaUnit - M_PI_2; //Variable only exists to minimize the #FP operations
        double cosNextBeta = cos(nextBeta), sinNextBeta = sin(nextBeta);
        double nextCathetus = radius * cosNextBeta;
        auto nextY = (float) (radius * sinNextBeta), nextTY = 1.0f- (float) layer * textureYUnit;

        prototype.newStrip();

        for (int piece = 0; piece <= slices; piece++) {
            Point bottom = Point(
                    (float) (cathetus * sinsAlpha[piece]),
                    currY,
                    (float) (cathetus * cossAlpha[piece]),
                    (float) (sinsAlpha[piece] * cosCurrBeta),
                    (float) sinCurrBeta,
                    (float) (cossAlpha[piece] * cosCurrBeta),
                    Texture_Xs[piece],
                    currTY
            );

            Point top = Point(
                    (float) (nextCathetus * sinsAlpha[piece]),
                    nextY,
                    (float) (nextCathetus * cossAlpha[piece]),
                    (float) (sinsAlpha[piece] * cosNextBeta),
                    (float) sinNextBeta,
                    (float) (cossAlpha[piece] * cosNextBeta),
                    Texture_Xs[piece],
                    nextTY
            );
            prototype.autoPushSegment(top,bottom);

        }
        //Save some work for the next stack
        currY = nextY;
        currTY = nextTY;
        cosCurrBeta = cosNextBeta;
        sinCurrBeta = sinNextBeta;
        cathetus = nextCathetus;
    }
    return prototype;
}
