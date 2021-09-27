//
// Created by fouri on 2021/09/16.
//

#ifndef IMAGEPROCESSING_MIXING_H
#define IMAGEPROCESSING_MIXING_H

#include "../Color/Color.h"

class Mixing {
private:
public:
    static Matrix<double>* averageLab(Matrix<double>* pixels, int numPixels);

    static double standardDeviation(Matrix<double>* pixels, int numPixels);

    static double quantifyMixing(Matrix<double>* mixedPixels, int N, Matrix<double>* unmixedPixels, int M);
};


#endif //IMAGEPROCESSING_MIXING_H
