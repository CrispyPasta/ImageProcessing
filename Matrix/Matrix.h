//
// Created by fouri on 2021/09/14.
//

#ifndef IMAGEPROCESSING_MATRIX_H
#define IMAGEPROCESSING_MATRIX_H

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/matx.hpp>
#include "../Utility/Utility.h"

template <class T>
class Matrix {
private:
    void extendLine(Matrix& image, int r, int c, int i, char dir);

    void extendCorner(Matrix& image, int r, int c, int i, char dir);

public:
    int rows;
    int cols;
    T** mat;

    Matrix();

//    Matrix(int Rows, int Cols, double* data);

    Matrix(int Rows, int Cols, T* data);

    Matrix(int Rows, int Cols, T i = 0);

    Matrix(const Matrix& copy);

    static double convolve(Matrix& m1, Matrix& m2);

    static double convolve(Matrix& m1, cv::Mat& image, int r, int c, char chan);

    static double convolve(Matrix<int>& m1, int r, int c, Matrix<uint8_t>& m2, double t);

    static double convolve(Matrix<double>& m1, int r, int c, Matrix<uint8_t>& m2, Matrix<uint8_t>& out, double t);

    static Matrix* combineChannels(Matrix& red, Matrix& green, Matrix& blue);

    Matrix& operator+(const Matrix& m);

    Matrix& operator-(const Matrix& m) const;

    Matrix& operator*(const Matrix& m) const;

    void operator*=(int m) const;

    void operator*=(double m) const;

    void operator=(const Matrix& m);

    void operator=(int i);

    void operator=(double i);

    void expandMatrix(int i);

    void print(const std::string& caption = "", int width = 7, int precision = 3) const;

    void toArray(T* arr);

    cv::Mat* toMat();

    ~Matrix();
};


#endif //IMAGEPROCESSING_MATRIX_H
