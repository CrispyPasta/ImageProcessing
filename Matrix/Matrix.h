//
// Created by fouri on 2021/09/14.
//

#ifndef IMAGEPROCESSING_MATRIX_H
#define IMAGEPROCESSING_MATRIX_H


class Matrix {
public:
    int rows;
    int cols;
    double** mat;

    Matrix();

    Matrix(int Rows, int Cols, double* data);

    Matrix(int Rows, int Cols, double i = 0.0);

    Matrix(const Matrix& copy);

    static double convolve(Matrix& m1, Matrix& m2);

    Matrix& operator+(const Matrix& m);

    Matrix& operator-(const Matrix& m) const;

    Matrix& operator*(const Matrix& m) const;

//    Matrix& operator*(const double& d) const;

    void operator*=(int m) const;

    void operator*=(double m) const;

    void operator=(const Matrix& m);

    void operator=(int i);

    void operator=(double i);

    void print(const std::string& caption = "", int width = 7) const;

    ~Matrix();
};


#endif //IMAGEPROCESSING_MATRIX_H
