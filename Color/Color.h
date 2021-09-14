//
// Created by fouri on 2021/09/14.
//

#ifndef IMAGEPROCESSING_COLOR_H
#define IMAGEPROCESSING_COLOR_H


#include <opencv2/opencv.hpp>
#include "../Matrix/Matrix.h"

class Color {
private:
//    static Matrix<double> RGBtoXYZ;
public:


//    static inline Matrix<double> RGBtoXYZ = Matrix<double>();

    Color();
    ~Color();

    static double linearizeRGB(int C);

    static void rgbtoxyz(int* BGR);

    static void xyztoLab();

    static void rgbtoLab(cv::Mat& image);
};



#endif //IMAGEPROCESSING_COLOR_H
