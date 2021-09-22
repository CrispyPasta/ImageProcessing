//
// Created by fouri on 2021/09/21.
//

#ifndef IMAGEPROCESSING_EDGES_H
#define IMAGEPROCESSING_EDGES_H
#include "../Matrix/Matrix.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
using namespace cv;

class Edges {
private:
    static Matrix* getChannel(Mat& image, int i);
public:
    double sigma;
    int size;
    int k;
    double gaussianTotal;
    Matrix gaussianMatrix;

    Edges();

    Edges(int s);

    void generateGaussian(double sig = 1.4142135624);

    void gaussianBlur(Matrix& image);

    static Matrix* getRed(Mat& image);

    static Matrix* getBlue(Mat& image);

    static Matrix* getGreen(Mat& image);

    void print(std::string caption, int s = 5);

};


#endif //IMAGEPROCESSING_EDGES_H
