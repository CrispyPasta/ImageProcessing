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
#include <cmath>
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
    int pp;
    double gaussianTotal;
    double threshold_lower;
    Matrix<double> gaussianMatrix;

    Edges();

    Edges(int s);

    Edges(int s, double t_low);

    void generateGaussian(double sig = 1.4142135624);

    void gaussianBlur(Matrix<uint8_t>& image);

    int getSector(double angle);

    slope sobelPixel(Matrix<uint8_t>& image, int r, int c);

    void sobelImage(Matrix<uint8_t>& image, slope** output);

    bool isLocalMax(edgePixel** image, int u, int v);

    void nonMaximumSuppression(edgePixel** image, int r, int c);

    static void maxMagnitudeGradient(edgePixel& pixel);

    static void maxMagnitudeGradient(edgePixel**, int r, int c);

    void traceAndThreshold(edgePixel** input,  Matrix<uint8_t>& output, int r, int c, int N, int M);

    void traceEdges(edgePixel** input,  Matrix<uint8_t>& output, int N, int M);

    edgePixel** slopesToEdges(slope** red, slope** green, slope** blue, int r, int c);

    static Matrix<uint8_t>* getRed(Mat& image);

    static Matrix<uint8_t>* getBlue(Mat& image);

    static Matrix<uint8_t>* getGreen(Mat& image);

    static Matrix<uint8_t>* toMatrix(Mat& image);

    void print(std::string caption, int s = 5);

};


#endif //IMAGEPROCESSING_EDGES_H
