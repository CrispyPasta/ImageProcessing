//
// Created by fouri on 2021/09/14.
//

#ifndef IMAGEPROCESSING_MATRIX_H
#define IMAGEPROCESSING_MATRIX_H

#include <string>

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

    Matrix(int Rows, int Cols, T i = 0.0);

    Matrix(const Matrix& copy);

    static double convolve(Matrix& m1, Matrix& m2);

    static double convolve(Matrix& m1, int r, int c, Matrix& m2, Matrix& out, double t);

    static double convolve(Matrix<double>& m1, int r, int c, Matrix<uint8_t>& m2, Matrix<uint8_t>& out, double t);

    Matrix& operator+(const Matrix& m);

    Matrix& operator-(const Matrix& m) const;

    Matrix& operator*(const Matrix& m) const;

//    Matrix& operator*(const double& d) const;

    void operator*=(int m) const;

    void operator*=(double m) const;

    void operator=(const Matrix& m);

    void operator=(int i);

    void operator=(double i);

    void expandMatrix(int i);

    void print(const std::string& caption = "", int width = 7, int precision = 3) const;

    void toArray(T* arr);

    ~Matrix();
};


#endif //IMAGEPROCESSING_MATRIX_H
