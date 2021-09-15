//
// Created by fouri on 2021/09/14.
//
#ifndef IMAGEPROCESSING_COLOR_H
#define IMAGEPROCESSING_COLOR_H

#include <opencv2/opencv.hpp>
#include "../Matrix/Matrix.h"

class Color {
public:
    Color();
    ~Color();

    static double linearizeRGB(int C);

    static void rgbtoXyz(Matrix& BGR);

    static void xyztoLab(Matrix& xyz);

    static Matrix* rgbtoLab(int* image);

    static double deltaE(Matrix& val1, Matrix& val2);

    static std::string classifyColor(Matrix& val);
};

#endif //IMAGEPROCESSING_COLOR_H
