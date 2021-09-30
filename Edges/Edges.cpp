//
// Created by fouri on 2021/09/21.
//

#include "Edges.h"
#include <math.h>
#include <iostream>
using namespace std;

/**
 * The default constructor will generate a 7x7 matrix for the gaussian (k = 3).
 */
Edges::Edges() {
    size = 7;
    k = 3;
    gaussianMatrix = Matrix<double>(size, size);

    int KxList[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int KyList[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    Kx = Matrix<int>(3, 3, KxList);
    Ky = Matrix<int>(3, 3, KyList);
}

/**
 * Instantiates the gaussian matrix using the sizes provided
 * @param s : The literal size of the matrix. It will be (s x s)-sized. K will be set to (s / 2) - 1
 */
Edges::Edges(int s) {
    if (s % 2 != 1){
        cout << "The size of the kernel should be an odd number.\n";
    }
    size = s;
    k = (s - 1) / 2;
    //matrix is an array of pointers
    gaussianMatrix = Matrix<double>(s, s);
    int KxList[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int KyList[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    Kx = Matrix<int>(3, 3, KxList);
    Ky = Matrix<int>(3, 3, KyList);
}

/**
 * Generates the gaussian blur matrix using the given sigma value.
 * @param sig : The sigma value to be used in the generation of values. Sqrt(2) is popular.
 */
void Edges::generateGaussian(double sig) {
    if ((2 * k + 1) != size){
        cout << "This kernel's k value is incorrect. 2K + 1 ≠ Size. Aborting gaussian function." << endl;
        return;
    }
    sigma = sig;
    //calculate the constants before the loop
    double a;   //temp value 1
    double b;   //temp value 2
    double norm = 1.0 / (2.0 * M_PI * sigma * sigma);
    norm = 90;
    double d = 2.0 * sigma * sigma;

    for (int i = 1; i <= (2*k + 1); i++){
        a = pow(i - (k + 1), 2);
        for (int j = 1; j <= (2*k + 1); j++){
            b = pow(j - (k + 1), 2);    //dont doubt the intervals or the values for a and b, they make sense.
            gaussianMatrix.mat[i - 1][j - 1] = round(norm * exp(-((a + b) / d)));
            gaussianTotal += round(norm * exp(-((a + b) / d)));
//            gaussianMatrix.mat[i - 1][j - 1] = norm * exp(-((a + b) / d));
        }
    }
}

/**
 * This function will convolve the input matrix (an image) with the gaussian matrix.
 * @param image : The matrix containing the image we want to blur.
 */
void Edges::gaussianBlur(Matrix<uint8_t> &image) {
    int rows = image.rows;
    int cols = image.cols;
    Matrix<uint8_t>* blurredImage = new Matrix<uint8_t>(image.rows, image.cols);
    image.expandMatrix(k);      //expand the image

    double d;
    for (int a = 0; a < rows; a++){
        for (int b = 0; b < cols; b++){
            Matrix<uint8_t>::convolve(gaussianMatrix, a, b, image, *blurredImage, gaussianTotal);
        }
    }

    image.rows = blurredImage->rows;
    image.cols = blurredImage->cols;
    delete image.mat;
    image.mat = blurredImage->mat;
}

/**
 * Performs the sobel operation on a given image kernel (3x3 pixels).
 * @param image : The portion of the image for which the Sobel operation must be used.
 * @return : A slope struct containing the magnitude and direction of the slope at that point.
 */
slope Edges::sobelPixel(Mat &image, int r, int c) {
    int Ix = Matrix<int>::convolve(Kx, image, r, c, 'n');
    int Iy = Matrix<int>::convolve(Ky, image, r, c, 'n');

    float G = sqrt(Ix * Ix + Iy * Iy);
    float sig = atan(Iy / Ix) * 57.29577951;        //convert to degrees
    return {G, sig};
}

/**
 * Calculates the slopes for the entire image using the sobel operator. Assuming the image is expanded by more than just
 * one pixel, the p parameter lets you set the position where the function starts calculating the slope.
 * @param image : The mat image object.
 * @param output : A 2D array of slope structs
 * @param p : How many pixels were added to the image when it was expanded.
 */
void Edges::sobelImage(Mat &image, slope **output, int p) {
    try{
        int rows = image.rows;
        int cols = image.cols;
        int offset = p - 1;

        for (int a = 0; a < rows - p - p; a++){
            for (int b = 0; b < cols - p - p; b++){
                output[a][b] = sobelPixel(image, a + offset, b + offset);
            }
        }
    } catch (...) {
        string e = "An error was encountered in the sobelImage function.\n";
        throw e;
    }
}

/**
 * Returns an image containing only the desired color channel's data. i determines which channel is returned.
 * @param image : The image (opencv:mat object)
 * @param i : 2 for red, 1 for green and 0 for blue
 * @return : Matrix* a new image object that has only the desired color data in it.
 */
Matrix<uint8_t>* Edges::getChannel(Mat &image, int i) {
    auto* imageChannel = new Matrix<uint8_t>(image.rows, image.cols);
    Vec3b intensity;

    for (int a = 0; a < imageChannel->rows; a++){
        for (int b = 0; b < imageChannel->cols; b++){
            intensity = image.at<Vec3b>(a, b);
            imageChannel->mat[a][b] = intensity.val[i];
        }
    }

    return imageChannel;
}

Matrix<uint8_t>* Edges::getRed(Mat &image) {
    return getChannel(image, 2);
}

Matrix<uint8_t>* Edges::getGreen(Mat &image) {
    return getChannel(image, 1);
}

Matrix<uint8_t>* Edges::getBlue(Mat &image) {
    return getChannel(image, 0);
}

Matrix<uint8_t> *Edges::toMatrix(Mat &image) {
    auto* imageChannel = new Matrix<uint8_t>(image.rows, image.cols);
    uint8_t intensity;

    for (int a = 0; a < imageChannel->rows; a++){
        for (int b = 0; b < imageChannel->cols; b++){
            intensity = image.at<uint8_t>(a, b);
            imageChannel->mat[a][b] = intensity;
        }
    }

    return imageChannel;
}

/**
 * Prints the gaussian matrix used by the edge detection function
 * @param caption : The caption that should be printed
 * @param s : The column size for the matrix
 */
void Edges::print(string caption, int s) {
    gaussianMatrix.print(caption, s);
}
