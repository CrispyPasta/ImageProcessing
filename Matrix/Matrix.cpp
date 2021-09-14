//
// Created by fouri on 2021/08/22.
//
#include <iostream>
#include <iomanip>
#include "Matrix.h"
using namespace std;

template <typename T>
Matrix<T>::Matrix() {
    *this = Matrix(3, 3, 0.0);
}

template <typename T>
Matrix<T>::Matrix(int Rows, int Cols, T* data){
    rows = Rows;
    cols = Cols;
    mat = new T*[rows];
    for (int a = 0; a < rows; a++){
        mat[a] = new T[cols];
    } //don't doubt this my boy

    int i = 0;  //counter to iterate through the data
    for (int a = 0; a < Rows; a++){
        for (int b = 0; b < Cols; b++){
            if (i >= Rows * Cols){
                mat[a][b] = 0;
            } else {
                mat[a][b] = data[i++];
            }
        }
    }
}

//constructor
template <typename T>
Matrix<T>::Matrix(int r, int c, T i) {
    rows = r;
    cols = c;

    mat = new T*[rows];
    for (int a = 0; a < rows; a++){
        mat[a] = new T[cols];
    } //don't doubt this my boy

    for (int a = 0; a < rows; a++){
        for (int b = 0; b < cols; b++){
            mat[a][b] = i;
        }
    }
}

//copy constructor
template <typename T>
Matrix<T>::Matrix(const Matrix &copy) {
    this->rows  = copy.rows;
    this->cols  = copy.cols;

    this->mat = new T*[rows];
    for (int a = 0; a < cols; a++){
        mat[a] = new T[cols];
    }

    for (int a = 0; a < rows; a++){
        for (int b = 0; b < cols; b++){
            mat[a][b] = copy.mat[a][b];
        }
    }
}

//convolution operator where M1 is the Kernel and M2 is the image
//returns one value because convolution returns a scalar value and not another matrix
template <typename T>
double Matrix<T>::convolve(Matrix &m1, Matrix &m2) {
    if ((m1.rows != m1.cols) || (m2.rows != m2.cols)){
        string error = "* * * * * * * ERROR * * * * * * *\n";
        error += "One or both of the matrices are asymmetrical.\n";
        throw error;
    }
    if ((m1.rows != m2.rows) || (m1.cols != m2.cols)){
        string error = "* * * * * * * ERROR * * * * * * *\n";
        error += "The matrices submitted do no have the same dimensions:\nM1: [";
        error += to_string(m1.rows) + ", " + to_string(m1.cols) + "]";
        error += ", M2: [";
        error += to_string(m2.rows) + ", " + to_string(m2.cols) + "]\n";
        throw error;
    }

    double result = 0.0;
    int m = m1.rows;
    int n = m;  //if the matrices have equal dimensions, then m = n

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            result += m1.mat[m - i - 1][n - j - 1] * m2.mat[i][j];
        }
    }

    return result;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+(const Matrix &m) {
    if ((this->rows != m.rows) || (this->cols != m.cols)){
        string message = "";
        message += "The dimensions of the matrices don't match: ";
        cout << message;
        message += "Matrix 1: [" + to_string(this->rows) + ", " + to_string(this->cols) + "]";
        message += "Matrix 2: [" + to_string(m.rows) + ", " + to_string(m.cols) + "]";
        throw message;  //stop the program so we don't waste time or destroy data or something
    } else {
        Matrix* result = new Matrix(m.rows, m.cols);         //this will hold the answer, pass by value to ease mem management
        for (int a = 0; a < m.rows; a++){
            for (int b = 0; b < m.cols; b++){
                result->mat[a][b] = this->mat[a][b] + m.mat[a][b];
            }
        }
        return *result;
    }
}

template <typename T>
Matrix<T> &Matrix<T>::operator-(const Matrix &m) {
    if ((this->rows != m.rows) || (this->cols != m.cols)){
        string message = "";
        message += "The dimensions of the matrices don't match: ";
        cout << message;
        message += "Matrix 1: [" + to_string(this->rows) + ", " + to_string(this->cols) + "]";
        message += "Matrix 2: [" + to_string(m.rows) + ", " + to_string(m.cols) + "]";
        throw message;  //stop the program so we don't waste time or destroy data or something
    } else {
        Matrix* result = new Matrix(m.rows, m.cols);         //this will hold the answer, pass by value to ease mem management
        for (int a = 0; a < m.rows; a++){
            for (int b = 0; b < m.cols; b++){
                result->mat[a][b] = this->mat[a][b] - m.mat[a][b];
            }
        }
        return *result;
    }
}

//matrix multiplication function
template <typename T>
Matrix<T> &Matrix<T>::operator*(const Matrix &m) {
    if (this->cols != m.rows){
        string error = "* * * * * * * ERROR * * * * * * *\n";
        error += "Matrix dimensions incompatible for multiplication.\n";
        throw error;
    } else {
        try{
            auto result = new Matrix(this->rows, m.cols);
            int i, j, k;
            for (i = 0; i < this->rows; i++){
                for (j = 0; j < m.cols; j++){
                    for (k = 0; k < this->cols; k++){
                        result->mat[i][j] += this->mat[i][k] * m.mat[k][j];
                    }
                }
            }
            return *result;
        } catch (...){
            string error = "Error encountered in multiplication loop.\n";
            throw error;
        }
    }
}

//multiply matrix by constant value
template <typename T>
void Matrix<T>::operator*=(int m) {
    for (int a = 0; a < this->rows; a++){
        for (int b = 0; b < this->cols; b++){
            this->mat[a][b] *= m;
        }
    }
}

//multiply matrix by constant value
template <typename T>
void Matrix<T>::operator*=(double m) {
    for (int a = 0; a < this->rows; a++){
        for (int b = 0; b < this->cols; b++){
            this->mat[a][b] *= m;
        }
    }
}

//assignment operator (assign matrix to other matrix)
template <typename T>
void Matrix<T>::operator=(const Matrix &m) {
    this->rows = m.rows;
    this->cols = m.cols;

    this->mat = new T*[rows];
    for (int a = 0; a < cols; a++){
        this->mat[a] = new T[cols];
    }

    for (int a = 0; a < rows; a++){
        for (int b = 0; b < cols; b++){
            this->mat[a][b] = m.mat[a][b];
        }
    }
}

//Sets all values in the matrix to the value given
template <typename T>
void Matrix<T>::operator=(const int i) {
    for (int a = 0; a < this->rows; a++){
        for (int b = 0; b < this->cols; b++){
            this->mat[a][b] = i;
        }
    }
}

//Sets all values in the matrix to the value given
template <typename T>
void Matrix<T>::operator=(const double d) {
    for (int a = 0; a < this->rows; a++){
        for (int b = 0; b < this->cols; b++){
            this->mat[a][b] = d;
        }
    }
}

template <typename T>
void Matrix<T>::print(const string& caption, int width) const{
    cout << caption << endl;
    for (int a = 0; a < rows; a++){
        cout << left;
        cout << "| ";
        for (int b = 0; b < cols; b++){
            cout << setw(width) << to_string(mat[a][b]) << " ";
        }
        cout << " |" << endl;
    }
}

template <typename T>
Matrix<T>::~Matrix() {
    for (int a = 0; a < rows; a++){
        delete mat[a];
    }
    delete mat;
}

template class Matrix<int>;
template class Matrix<double>;
