//
// Created by fouri on 2021/09/21.
//

#ifndef IMAGEPROCESSING_EDGES_H
#define IMAGEPROCESSING_EDGES_H
#include "../Matrix/Matrix.h"
#include "../Utility/Utility.h"
#include <opencv2/opencv.hpp>

#include <opencv2/core/matx.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include <math.h>
using namespace cv;

class Edges {
private:
    static Matrix<uint8_t>* getChannel(Mat& image, int i);

    int getOrientationSector(double Ix, double Iy);
    Matrix<int> Kx;
    Matrix<int> Ky;
    double rotationMatrix[4] = {0.923879, -0.382683, 0.382683, 0.923879};
public:
    double sigma;
    int size;
    int k;
    double gaussianTotal;
    Matrix<double> gaussianMatrix;

    Edges();

    Edges(int s);

    void generateGaussian(double sig = 1.4142135624);

    void gaussianBlur(Matrix<uint8_t>& image);

    slope sobelPixel(Mat& image, int r, int c);

    void nonMaximumSuppression(slope** image, int r, int c);

    int getSector(double angle);

    void sobelImage(Mat& image, slope** output, int p);

    static void maxMagnitudeGradient(edgePixel**, int r, int c);

    static void maxMagnitudeGradient(edgePixel& pixel);

    static Matrix<uint8_t>* getRed(Mat& image);

    static Matrix<uint8_t>* getBlue(Mat& image);

    static Matrix<uint8_t>* getGreen(Mat& image);

    static Matrix<uint8_t>* toMatrix(Mat& image);

    void print(std::string caption, int s = 5);

};


#endif //IMAGEPROCESSING_EDGES_H
