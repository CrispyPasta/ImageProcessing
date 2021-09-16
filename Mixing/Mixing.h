//
// Created by fouri on 2021/09/16.
//

#ifndef IMAGEPROCESSING_MIXING_H
#define IMAGEPROCESSING_MIXING_H

#include "../Color/Color.h"

class Mixing {
private:
public:
    static Matrix* averageLab(Matrix* pixels, int numPixels);

    static double standardDeviation(Matrix* pixels, int numPixels);

    static double quantifyMixing(Matrix* mixedPixels, int N, Matrix* unmixedPixels, int M);
};


#endif //IMAGEPROCESSING_MIXING_H
