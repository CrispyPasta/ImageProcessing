//
// Created by fouri on 2021/09/16.
//

#ifndef IMAGEPROCESSING_MIXING_H
#define IMAGEPROCESSING_MIXING_H

#include "../Color/Color.h"
#include <thread>
#include <future>

class Mixing {
private:
    static void getAverage(std::promise<float> total, Matrix<double> *p, int len, int chan);
public:
    static Matrix<double>* averageLab(Matrix<double>* pixels, int numPixels);

    static double standardDeviation(Matrix<double>* pixels, int numPixels);

    static double quantifyMixing(Matrix<double>* mixedPixels, int N, Matrix<double>* unmixedPixels, int M);

    static double quantifyMixing(Matrix<double> *mixedPixels, int N, Matrix<double> *unmixedPixels, int M, Matrix<double> * unmixed2, int Z);
};


#endif //IMAGEPROCESSING_MIXING_H
