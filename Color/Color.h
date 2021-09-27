//
// Created by fouri on 2021/09/14.
//
#ifndef IMAGEPROCESSING_COLOR_H
#define IMAGEPROCESSING_COLOR_H

#include "../Matrix/Matrix.h"
#include <string>
#include <opencv2/opencv.hpp>

class Color {
public:
    Color();
    ~Color();

    static double linearizeRGB(int C);

    static void rgbtoXyz(Matrix<double>& BGR);

    static void xyztoLab(Matrix<double>& xyz);

    static Matrix<double>* rgbtoLab(int* image);

    static double deltaE(Matrix<double>& val1, Matrix<double>& val2);

    static std::string classifyColor(Matrix<double>& val);
};

#endif //IMAGEPROCESSING_COLOR_H
