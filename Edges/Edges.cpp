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
    pp = 0;
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

    threshold_lower = 20;       //random value
}

Edges::Edges(int s, double t) {
    if (s % 2 != 1){
        cout << "The size of the kernel should be an odd number.\n";
    }
    size = s;
    k = (s - 1) / 2;
    pp = 0;
    //matrix is an array of pointers
    gaussianMatrix = Matrix<double>(s, s);
    int KxList[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int KyList[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    Kx = Matrix<int>(3, 3, KxList);
    Ky = Matrix<int>(3, 3, KyList);
    threshold_lower = t;
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
    norm *= 91;
    double d = 2.0 * sigma * sigma;

    for (int i = 1; i <= (2*k + 1); i++){
        a = pow(i - (k + 1), 2);
        for (int j = 1; j <= (2*k + 1); j++){
            b = pow(j - (k + 1), 2);    //don't doubt the intervals or the values for a and b, they make sense.
            gaussianMatrix.mat[i - 1][j - 1] = (norm * exp(-((a + b) / d)));
            gaussianTotal += (norm * exp(-((a + b) / d)));
            // gaussianMatrix.mat[i - 1][j - 1] = norm * exp(-((a + b) / d));
        }
    }

    gaussianMatrix.print("Gaussian generated:", 5);
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

    double d;         //convolve the image with the gaussian and store the blurred data in blurredImage
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

/**
 * This function takes the angle of the gradient of a pixel and returns the sector it falls into 
 * (0, 1, 2 or 3).
 * @param angle : The angle of the gradient in degrees.
 * @return : 0, 1, 2 or 3, indicating the sector 
 */
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
}

/**
 * Performs the sobel operation on a given image kernel (3x3 pixels).
 * @param image : The portion of the image for which the Sobel operation must be used.
 * @return : A slope struct containing the magnitude and direction of the slope at that point.
 */
slope Edges::sobelPixel(Matrix<uint8_t>& image, int r, int c) {
    int Ix = round(Matrix<int>::convolve(Kx, r, c, image, 8.0));     //convolve and normalize
    int Iy = round(Matrix<int>::convolve(Ky, r, c, image, 8.0));

    double G = sqrt(Ix * Ix + Iy * Iy);
    double sig = atan2(Iy, Ix) * 57.29577951;        //convert to degrees
    sig  = getSector(sig);
    // double sig = getOrientationSector(Ix, Iy);
    return {G, sig};
}

/**
 * Calculates the slopes for the entire image using the sobel operator. Assuming the image is expanded by more than just
 * one pixel, the p parameter lets you set the position where the function starts calculating the slope.
 * @param image : The mat image object.
 * @param output : A 2D array of slope structs
 */
void Edges::sobelImage(Matrix<uint8_t>& image, slope **output) {
    try{
        int rows = image.rows;
        int cols = image.cols;

        for (int a = 0; a < rows - 2; a++){    //the sobel matrix is 3x3, so don't do sobelPixel on the very edge pixels
            for (int b = 0; b < cols - 2; b++){
                output[a][b] = sobelPixel(image, a, b);
            }
        }
    } catch (...) {
        string e = "An error was encountered in the sobelImage function.\n";
        throw e;
    }
}

/**
 * Takes the combined magnitude and direction data for all the channels after it has been combined into one (in my case,
 * finding the biggest magnitude of the three channels and using that one's data), the position of the pixel we want to
 * look at, the direction of that pixel and the lower threshold that we use to determine eligibility for being an edge
 * pixel. Returns true if the pixel is a local maximum.
 * @param image : 2D array of edgePixel structs, containing the gradient data for all the channels of the image.
 * @param u : the row of the pixel we're checking
 * @param v : the col of the pixel we're checking
 * @param direction : the direction (0, 1, 2 or 3) of the gradient at that pixel
 * @param threshold_low : the lower threshold for eligibility of a local max.
 * @return True is the pixel is a local maximum, False otherwise.
 */
bool Edges::isLocalMax(edgePixel **image, int u, int v) {
    double centerPixel = image[u][v].maxMag;            //center pixel, use its magnitude

    if (pp == 0){
        cout << "Non max suppression is underway\n";
        // Utility::print(image, 12, 14);
        // cout << "Non max suppression is underway\n\n";
    }
    pp++;

    if (centerPixel < threshold_lower){
        return false;
    } else {
        double lPixel;
        double rPixel;
        char direction = image[u][v].maxAngle;
        switch (direction) {
            case 0:
                lPixel = image[u][v - 1].maxMag;
                rPixel = image[u][v + 1].maxMag;
                break;
            case 1:
                lPixel = image[u - 1][v - 1].maxMag;
                rPixel = image[u + 1][v + 1].maxMag;
                break;
            case 2:
                lPixel = image[u - 1][v].maxMag;
                rPixel = image[u + 1][v].maxMag;
                break;
            case 3:
                lPixel = image[u - 1][v + 1].maxMag;
                rPixel = image[u + 1][v - 1].maxMag;
                break;
            default:
                string e = "Invalid direction given.\n";
                throw e;
        }

        if ((centerPixel >= lPixel) && (centerPixel >= rPixel)){
            return true;
        } else {
            return false;
        }
    }
}

/**
 * Performs non-maxima suppression on the portion of the image given, indicated by r and c.
 * @param image : The array of slopes and magnitudes of the image (2D slope array)
 * @param r : number of rows in the image
 * @param c : number of cols in the image
 */
void Edges::nonMaximumSuppression(edgePixel** image, int r, int c) {
    bool maxPixels[r-1-1][c-1-1];

    for (int a = 1; a < r - 1; a++){
        for (int b = 1; b < c - 1; b++){
            maxPixels[a - 1][b - 1] = isLocalMax(image, a, b);
            if (!maxPixels[a - 1][b - 1]) {      //if the pixel is not a maximum, set it to zero.
                image[a][b].maxMag = 0;
            }   //if the pixel IS a local maximum, let it keep its magnitude.
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
            p.maxMag = p.Rmag;
            p.maxAngle = p.Rdir;
        } else {
            p.maxMag = p.Gmag;
            p.maxAngle = p.Gdir;
        }
    } else {
        if (p.Bmag >= p.Gmag){
            p.maxMag = p.Bmag;
            p.maxAngle = p.Bdir;
        } else {
            p.maxMag = p.Gmag;
            p.maxAngle = p.Gdir;
        }
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
 * Performs the trace and threshold step for edge detection. Takes the input matrix (which comes from the non maximum
 * suppression function), a new output matrix (initialized to all zeros) and some ints. Choose a starting point (r, c)
 * and give the size of the input matrix using N and M.
 * @param input : 2D array of edgePixel structs, should come from nonMaxSuppression step.
 * @param output : 2D array of uint8_t, initialized to all zeros. This is the final output of edge detection.
 * @param r : Starting row for this step
 * @param c : Starting col for this step
 * @param N : Number of rows in the input matrix
 * @param M : Number of cols in the input matrix.
 */
void Edges::traceAndThreshold(edgePixel **input, Matrix<uint8_t>& output, int r, int c, int N, int M) {
    int cLeft = max(c - 1, 0);
    int cRight = min(c + 1, M - 1);
    int rTop = max(r - 1, 0);
    int rBtm = min(r + 1, N - 1);

    for (int a = rTop; a <= rBtm; a++){
        for (int b = cLeft; b <= cRight; b++){
            double tmp = input[a][b].maxMag;
            if ((input[a][b].maxMag >= threshold_lower) && (output.mat[a][b] == 0)){
                output.mat[r][c] = 255;
                traceAndThreshold(input, output, a, b, N, M);
            }
        }
    }
}

/**
 * This is basically just a wrapper for the traceAndThreshold function so that wherever that step needs to
 * be done, it will look tidier. This function calls traceAndThreshold for each pixel in the given input matrix.
 * @param input : The edgePixel array that holds the maximum suppressed slopes of the image. 
 * @param output : A matrix object that will eventually be stored or used as a binary image. Final output of the process.
 * @param N : The number of rows in the output matrix 
 * @param output : The number of cols in the output matrix 
 */
void Edges::traceEdges(edgePixel **input, Matrix<uint8_t>& output, int N, int M) {
    for (int a = 1; a < N - 1; a++){
        for (int b = 1; b < M - 1; b++){
            this->traceAndThreshold(input, output, a, b, N, M);
        }
    }
}

/**
 * Just a function to convert a trio of 2D slope arrays into one array of edgePixel structs.
 */
edgePixel **Edges::slopesToEdges(slope** red, slope** green, slope** blue, int r, int c) {
    auto output = new edgePixel*[r];
    for (int a = 0; a < r; a++){
        output[a] = new edgePixel[c];
        for (int b = 0; b < c; b++){
            output[a][b] = {red[a][b].magnitude, red[a][b].direction,
                            green[a][b].magnitude, green[a][b].direction,
                            blue[a][b].magnitude, blue[a][b].direction};

            // Utility::print(output[a][b]);
            // cout << endl;
        }
    }
    // Utility::print(output, r, c);
    return output;
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
