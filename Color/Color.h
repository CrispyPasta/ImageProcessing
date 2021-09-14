//
// Created by fouri on 2021/09/14.
//

#ifndef IMAGEPROCESSING_COLOR_H
#define IMAGEPROCESSING_COLOR_H


#include <opencv2/opencv.hpp>
#include "../Matrix/Matrix.h"

class Color {
private:
    Matrix<double> RGBtoXYZ = Matrix<double>(3, 3);

    double RGBtoXYZmat[3][3] = {
            {0.4124, 0.3576, 0.1805},
            {0.2126, 0.7152, 0.0722},
            {0.0193, 0.1192, 0.9505},
            };
public:
    Color();
    ~Color();

    static double linearizeRGB(int C);

    static void rgbtoxyz(int* BGR);

    static void xyztoLab();

    static void rgbtoLab(cv::Mat& image);
};



#endif //IMAGEPROCESSING_COLOR_H
