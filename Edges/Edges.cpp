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
    gaussianMatrix = Matrix(size, size);
}

/**
 * Instantiates the gaussian matrix using the sizes provided
 * @param s : The literal size of the matrix. It will be (s x s)-sized. K will be set to (s / 2) - 1
 */
Edges::Edges(int s) {
    size = s;
    k = (s - 1) / 2;
    //matrix is an array of pointers
    gaussianMatrix = Matrix(s, s);
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
//            gaussianMatrix.mat[i - 1][j - 1] = norm * exp(-((a + b) / d));
        }
    }
}

/**
 * This function will convolve the input matrix (an image) with the gaussian matrix.
 * @param image : The matrix containing the image we want to blur.
 */
void Edges::gaussianBlur(Matrix &image) {
    int rows = image.rows;
    int cols = image.cols;
    Matrix* blurredImage = new Matrix(image.rows, image.cols);
    image.expandMatrix(k);      //expand the image

    double d;
    for (int a = 0; a < rows; a++){
        for (int b = 0; b < cols; b++){
            Matrix::convolve(gaussianMatrix, a, b, image, *blurredImage);
        }
    }

    image.rows = blurredImage->rows;
    image.cols = blurredImage->cols;
    delete image.mat;
    image.mat = blurredImage->mat;
}

/**
 * Prints the gaussian matrix used by the edge detection function
 * @param caption : The caption that should be printed
 * @param s : The column size for the matrix
 */
void Edges::print(string caption, int s) {
    gaussianMatrix.print(caption, s);
}
