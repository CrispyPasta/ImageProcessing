//
// Created by fouri on 2021/09/14.
//

#ifndef IMAGEPROCESSING_MATRIX_H
#define IMAGEPROCESSING_MATRIX_H

template <typename T = double>      //default data type is double
class Matrix {
public:
    int rows;
    int cols;
    T** mat;

    Matrix();

    Matrix(int Rows, int Cols, T* data);

    Matrix(int Rows, int Cols, T i = 0.0);

    Matrix(const Matrix& copy);

    static double convolve(Matrix& m1, Matrix& m2);

    Matrix& operator+(const Matrix& m);

    Matrix& operator-(const Matrix& m);

    Matrix& operator*(const Matrix& m);

    void operator*=(int m);

    void operator*=(double m);

    void operator=(const Matrix& m);

    void operator=(int i);

    void operator=(double i);

    void print(const std::string& caption = "", int width = 7) const;

    ~Matrix();
};


#endif //IMAGEPROCESSING_MATRIX_H
