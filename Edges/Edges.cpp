//
// Created by fouri on 2021/09/21.
//

#include "Edges.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include "../Utility/Utility.h"
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
 * Performs non-maxima suppression on the portion of the image given, indicated by r and c.
 * @param image : The array of slopes and magnitudes of the image (2D slope array)
 * @param r : the row of the pixel being considered
 * @param c : the col of the pixel being considered
 */
void Edges::nonMaximumSuppression(slope** image, int r, int c) {
    int plusDelta[4][2] = {{1,0}, {1,1}, {0,1}, {-1,1}};
    int minDelta[4][2] = {{-1,0}, {-1,-1}, {0,-1}, {1,-1}};

    double t = abs(image[r][c].direction + 11.25);
    int direction = round(t / 22.5);
}

/**
 * Performs the sobel operation on a given image kernel (3x3 pixels).
 * @param image : The portion of the image for which the Sobel operation must be used.
 * @return : A slope struct containing the magnitude and direction of the slope at that point.
 */
slope Edges::sobelPixel(Mat &image, int r, int c) {
    int Ix = round(Matrix<int>::convolve(Kx, image, r, c, 'n') / 8.0);     //convolve and normalize
    int Iy = round(Matrix<int>::convolve(Ky, image, r, c, 'n') / 8.0);

    double G = sqrt(Ix * Ix + Iy * Iy);
    double sig = atan2(Iy, Ix) * 57.29577951;        //convert to degrees
    sig  = getSector(sig);
//    double sig = getOrientationSector(Ix, Iy);
    return {G, sig};
}

/**
 * This is supposed to be a more efficient way to get the sector of the gradient direction but I suspect that it's not
 * working correctly for some reason.
 * @param Ix
 * @param Iy
 * @return
 */
int Edges::getOrientationSector(double Ix, double Iy) {
    Matrix<double> rotator(2, 2, rotationMatrix);
    double tmp[] = {Ix, Iy};
    Matrix<double> gradient(2, 1, tmp);
    Matrix<double> rotatedMatrix = rotator * gradient;
    Ix = rotatedMatrix.mat[0][0];
    Iy = rotatedMatrix.mat[1][0];

    if (Iy < 0){
        Ix *= -1;
        Iy *= -1;
    }

    if ((Ix >= 0) && (Ix >= Iy)) {
        return 0;
    } else if ((Ix >= 0) && (Ix < Iy)) {
        return 1;
    } else if ((Ix < 0) && (-Ix < Iy)) {
        return 2;
    } else if ((Ix < 0) && (-Ix >= Iy)) {
        return 3;
    }

    return -1;
}

int Edges::getSector(double angle){
    if (angle < 0){
        angle += 180;
    }
    if (((angle >= 0) && (angle <= 22.5)) || ((angle <= 180) && (angle >= 157.5))){
        return 0;
    } else if ((angle > 22.5) && (angle <= 67.5)){
        return 3;
    } else if ((angle > 67.5) && (angle <= 112.5)){
        return 2;
    } else {
        return 1;
    }
    return -1;
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
 * Iterates over a 2D matrix of edgePixel structs and puts the greatest G value into the R channel.
 * That way, you only have to look at the red pixels to get the highest value for that pixel.
 * @param image : 2D array of edgepixel structs, containing the G value for R, G and B (after using sobel).
 * @param r : rows in the array
 * @param c : cols in the array
 */
void Edges::maxMagnitudeGradient(edgePixel** image, int r, int c) {
    for (int a = 0; a < r; a++){
        for (int b = 0; b < c; b++){
            maxMagnitudeGradient(image[a][b]);
        }
    }
}

/**
 * Compare the G values for R, G and B and set the "max" char accordingly. This is the best way to combine the data from
 * three color channels into one for edge detection.
 * @param p : an edgePixel struct containing the output from the sobel operation
 */
void Edges::maxMagnitudeGradient(edgePixel& p) {
    if (p.Rmag >= p.Bmag){
        if (p.Rmag >= p.Gmag){
            p.max = 'R';
            p.maxMag = p.Rmag;
            p.maxAngle = p.Rdir;
        } else {
            p.max = 'G';
            p.maxMag = p.Gmag;
            p.maxAngle = p.Gdir;
        }
    } else {
        if (p.Bmag >= p.Gmag){
            p.max = 'B';
            p.maxMag = p.Bmag;
            p.maxAngle = p.Bdir;
        } else {
            p.max = 'G';
            p.maxMag = p.Gmag;
            p.maxAngle = p.Gdir;
        }
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
