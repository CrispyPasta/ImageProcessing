//
// Created by fouri on 2021/09/16.
//

#include "Mixing.h"
#include <iomanip>
using namespace std;

/**
 * Calculates the standard deviation in an array of Lab pixels.
 * @param pixels : An array of Matrix objects, each of the dimensions 3x1
 * @param numPixels : The number of elements in the array
 * @return The standard deviation of the given pixels
 */
double Mixing::standardDeviation(Matrix<double> *pixels, int numPixels) {
    double sum = 0;

    Matrix<double> alpha = *averageLab(pixels, numPixels);  //alpha is the average pixel value
    double tmp;

    for (int a = 0; a < numPixels; a++){
        tmp = Color::deltaE(pixels[a], alpha);
        sum += tmp * tmp;     //add the delta E value to the sum
    }
    //now divide by numPixels
    sum /= numPixels;
    return sqrt(sum);   //and finally take the square root
}   //this function works (tested with calculator)


/**
 * Calculates the average Lab value for a given array of Lab pixels.
 * @param pixels : An array of Matrix objects, each of the dimensions 3x1
 * @param numPixels : Number of elements in the array
 * @return : A pixel containing the average values for each channel: L, a, and b
 */
Matrix<double>* Mixing::averageLab(Matrix<double> *pixels, int numPixels) {
    Matrix<double> sum(3, 1);
    auto* result = new Matrix<double>(3, 1);

    std::promise<float> L_ave;
//    std::promise<float> a_ave;
//    std::promise<float> b_ave;
//    auto f1 = L_ave.get_future();
//    auto f2 = a_ave.get_future();
//    auto f3 = b_ave.get_future();
//    std::thread t1(&Mixing::getAverage, std::move(L_ave), ref(pixels), numPixels, 0);
//    std::thread t2(&Mixing::getAverage, std::move(a_ave), ref(pixels), numPixels, 1);
//    std::thread t3(&Mixing::getAverage, std::move(b_ave), ref(pixels), numPixels, 2);
//    t1.join();
//    t2.join();
//    t3.join();
//    float L = f1.get();
//    float a_ = f2.get();
//    float b = f3.get();
//    cout << L << '\n' << a_ << '\n' << b << '\n';

    for (int a = 0; a < numPixels; a++){
        sum.mat[0][0] += pixels[a].mat[0][0];       //sum the L, a and b values
        sum.mat[1][0] += pixels[a].mat[1][0];
        sum.mat[2][0] += pixels[a].mat[2][0];
    }

    result->mat[0][0] = sum.mat[0][0] / numPixels;
    result->mat[1][0] = sum.mat[1][0] / numPixels;
    result->mat[2][0] = sum.mat[2][0] / numPixels;
//    result->mat[0][0] = L;
//    result->mat[1][0] = a_;
//    result->mat[2][0] = b;
    return result;
}

/**
 * Calculates the relative mixing index for a set of pixels given the current image and an image of
 * the unmixed state. Each image is represented by an array of pixels (matrix objects). RMI is returned
 * in the form of a percentage value.
 * @param mixedPixels : Array of Matrix objects representing the mixed image
 * @param N : Number of pixels in the mixedPixels array
 * @param unmixedPixels : Array of Matrix objects representing the non-mixed image
 * @param M : Number of pixels in the unmixedPixels array
 * @return The RMI as a percentage value (0-100)
 */
double Mixing::quantifyMixing(Matrix<double>* mixedPixels, int N, Matrix<double>* unmixedPixels, int M) {
    double mixedSD = standardDeviation(mixedPixels, N);
    double unmixedSD = standardDeviation(unmixedPixels, M);

    return (1.0 - (mixedSD / unmixedSD)) * 100.0;
}

double Mixing::quantifyMixing(Matrix<double> *mixedPixels, int N, Matrix<double> *unmixedPixels, int M, Matrix<double> * unmixed2, int Z) {
    double mixedSD = standardDeviation(mixedPixels, N);

    Matrix<double> unmixedList[M + Z];
    for (int a = 0; a < M + Z; a++) {
        if (a < M ) {
            unmixedList[a] = unmixedPixels[a];
        } else {
            unmixedList[a] = unmixed2[a - M];
        }
    }

    double unmixedSD = standardDeviation(unmixedList, M + Z);
    return (1.0 - (mixedSD / unmixedSD)) * 100.0;
}

void Mixing::getAverage(std::promise<float> total, Matrix<double>* p, int len, int chan) {
    float t = 0;
    for (int a = 0; a < len; a++) {
        t += p[a].mat[chan][0];
    }

    total.set_value(t / (len * 1.0));
}
//this works, confirmed with calculator
