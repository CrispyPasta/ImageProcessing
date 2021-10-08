#include <iostream>
#include <iomanip>
#include "Matrix.h"
using namespace std;
using namespace cv;

template<class T>
Matrix<T>::Matrix() {
    *this = Matrix(3, 3, 0.0);
}

//}

template<class T>
Matrix<T>::Matrix(int Rows, int Cols, T *data) {
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
template<class T>
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


template<class T>
Matrix<T>::Matrix(const Matrix &copy) {
    *this = Matrix(copy.rows, copy.cols, 0.0);

    for (int a = 0; a < rows; a++){
        for (int b = 0; b < cols; b++){
            this->mat[a][b] = copy.mat[a][b];
        }
    }
}

/**
 * Convolution operator where M1 is the Kernel and M2 is the image
 * @param m1 : The kernel (e.g. the gaussian matrix)
 * @param m2 : The image
 * @return Double (result of convolution)
 */
template<class T>
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
    int n = m;  //if the matrices have equal dimensions, then gaussianMatrix = n

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            result += m1.mat[m - i - 1][n - j - 1] * m2.mat[i][j];
        }
    }

    return result;
}

template<class T>
double Matrix<T>::convolve(Matrix &m1, cv::Mat &image, int r, int c, char chan) {
//    if ((m1.rows != m1.cols) || (image.rows != image.cols)){
//        string error = "* * * * * * * ERROR * * * * * * *\n";
//        error += "One or both of the matrices are asymmetrical.\n";
//        throw error;
//    }
//    if ((m1.rows != image.rows) || (m1.cols != image.cols)){
//        string error = "* * * * * * * ERROR * * * * * * *\n";
//        error += "The matrices submitted do no have the same dimensions:\nM1: [";
//        error += to_string(m1.rows) + ", " + to_string(m1.cols) + "]";
//        error += ", M2: [";
//        error += to_string(image.rows) + ", " + to_string(image.cols) + "]\n";
//        throw error;
//    }

    double result = 0.0;
    int m = m1.rows;
    int n = m;  //if the matrices have equal dimensions, then gaussianMatrix = n
    Vec3b intensity;
    int channel;

    switch (chan) {
        case 'r':
            channel = 2;
            break;
        case 'g':
            channel = 1;
            break;
        case 'b':
            channel = 0;
            break;
        default:
            channel = 0;
            break;
    }

    if (chan == 'n'){
        for (int i = 0; i < m; i++){
            for (int j = 0; j < n; j++){
                result += m1.mat[m - i - 1][n - j - 1] * image.at<uint8_t>(i + r, j + c);       //add offset
            }
        }
    } else {
        for (int i = 0; i < m; i++){
            for (int j = 0; j < n; j++){
                intensity = image.at<uint8_t>(i + r, j + c);
                result += m1.mat[m - i - 1][n - j - 1] * intensity.val[channel];                //add offset
            }
        }
    }

    return result;
}

/**
 * Calculates convolution where M1 is the kernel and M2 is the image, where the image matrix is larger than
 * the kernel. A subset of the image will be used to calculate the convolution. The result will be stored in
 * the Output matrix
 * @param m1 : The Kernel matrix (e.g. the gaussian)
 * @param r : The row of the cell where the subset of the image begins.
 * @param c : The col of the cell where the subset of the image begins.
 * @param m2 : The image matrix
 * @param out : The output matrix where the result of the convolution will be stored.
 * @return Double result of the calculation.
 */
 template<class T>
double Matrix<T>::convolve(Matrix<int>& m1, int r, int c, Matrix<uint8_t>& m2, double t) {
    if (!(m2.cols > m1.cols) || !(m2.rows > m1.rows)){
        string error = "* * * * * * * ERROR * * * * * * *\n";
        error += "The image matrix must be larger than the kernel.\n";
        throw error;
    }

    try{
        double result = 0.0;
        int m = m1.rows;
        int n = m;  //if the matrices have equal dimensions, then gaussianMatrix = n

        for (int i = 0; i < m; i++){
            for (int j = 0; j < n; j++){
                result += m1.mat[m - i - 1][n - j - 1] * m2.mat[r + i][c + j];
            }
        }

        return result / t;
    } catch (...){
        string e = "An error occurred in the convolve function.\n";
        throw e;
    }
}

template<class T>
double Matrix<T>::convolve(Matrix<double>& m1, int r, int c, Matrix<uint8_t>& m2, Matrix<uint8_t>& out, double t) {
    if (m2.cols <= m1.cols || m2.rows <= m1.rows) {
        string error = "* * * * * * * ERROR * * * * * * *\n";
        error += "The image matrix must be larger than the kernel.\n";
        throw error;
    }

    double result = 0.0;
    int m = m1.rows;
    int n = m;  //if the matrices have equal dimensions, then gaussianMatrix = n

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            result += m1.mat[m - i - 1][n - j - 1] * m2.mat[r + i][c + j];
        }
    }

    out.mat[r][c] = result / t;

    return result / t;
}

/**
 * Take the red, green and blue matrix objects and combine them into one to make a whole image again.
 * @tparam T :  Preferably
 * @param red : Matrix containing red channel
 * @param green : Matrix containing green channel
 * @param blue : Matrix containing blue channel
 * @return A new matrix containing all three channels in the BGR order
 */
template<class T>
Matrix<T> *Matrix<T>::combineChannels(Matrix &red, Matrix &green, Matrix &blue) {
    try{
        bool sameSize = false;
        if ((red.rows == green.rows) && (green.rows == blue.rows)){
            if ((red.cols == green.cols) && (green.cols == blue.cols)){
                sameSize = true;
            }
        }
        if (!sameSize){
            string e = "The red, green and blue matrix objects don't have the same dimensions.\n";
            e += "Red: " + to_string(red.rows) + "x" + to_string(red.cols) + '\t';
            e += "Green: " + to_string(green.rows) + "x" + to_string(green.cols) + '\t';
            e += "Blue: " + to_string(blue.rows) + "x" + to_string(blue.cols);
            throw e;
        } else {
            auto* BGRimage = new Matrix<T>(red.rows, red.cols * 3);

            for (int a = 0; a < BGRimage->rows; a++){
                for(int b = 0; b < red.cols; b++){
                    BGRimage->mat[a][b * 3] = blue.mat[a][b];
                    BGRimage->mat[a][b * 3 + 1] = green.mat[a][b];
                    BGRimage->mat[a][b * 3 + 2] = red.mat[a][b];
                }
            }
            return BGRimage;
        }
    } catch (...) {
        string e = "An error occurred in the combineChannels function.\n";
        cout << endl << e;
        throw e;
    }
}

template<class T>
Matrix<T>& Matrix<T>::operator+(const Matrix &m) {
    if ((this->rows != m.rows) || (this->cols != m.cols)){
        string message = "";
        message += "The dimensions of the matrices don't match: ";
        message += "Matrix 1: [" + to_string(this->rows) + ", " + to_string(this->cols) + "]";
        message += "Matrix 2: [" + to_string(m.rows) + ", " + to_string(m.cols) + "]";
        cout << message;
        throw message;  //stop the program so we don't waste time or destroy data or something
    } else {
        Matrix<T>* result = new Matrix<T>(m.rows, m.cols);         //this will hold the answer, pass by value to ease mem management
        for (int a = 0; a < m.rows; a++){
            for (int b = 0; b < m.cols; b++){
                result->mat[a][b] = this->mat[a][b] + m.mat[a][b];
            }
        }
        return *result;
    }
}

template<class T>
Matrix<T> &Matrix<T>::operator-(const Matrix &m) const {
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
template<class T>
Matrix<T> &Matrix<T>::operator*(const Matrix &m) const {
    if (this->cols != m.rows){
        string error = "* * * * * * * ERROR * * * * * * *\n";
        error += "Matrix dimensions incompatible for multiplication.\n";
        throw error;
    } else {
        try{
            Matrix* result = new Matrix(this->rows, m.cols);
//            result->print("Result matrix: matrix multiplication");
            int i, j, k;
            for (i = 0; i < this->rows; ++i){
                for (j = 0; j < m.cols; ++j){
                    for (k = 0; k < this->cols; ++k){
                        result->mat[i][j] += this->mat[i][k] * m.mat[k][j];
                    }
                }
            }
//            result->print();
            return *result;
        } catch (...){
            string error = "Error encountered in multiplication loop.\n";
            throw error;
        }
    }
}

//multiply matrix by constant value
template<class T>
void Matrix<T>::operator*=(int m) const {
    for (int a = 0; a < this->rows; a++){
        for (int b = 0; b < this->cols; b++){
            this->mat[a][b] *= m;
        }
    }
}

//multiply matrix by constant value
template<class T>
void Matrix<T>::operator*=(double m) const {
    for (int a = 0; a < this->rows; a++){
        for (int b = 0; b < this->cols; b++){
            this->mat[a][b] *= m;
        }
    }
}

//assignment operator (assign matrix to other matrix)
template<class T>
void Matrix<T>::operator=(const Matrix &m) {
    this->rows = m.rows;
    this->cols = m.cols;

    this->mat = new T*[rows];
    for (int a = 0; a < rows; a++){
        this->mat[a] = new T[cols];
    }

    for (int a = 0; a < rows; a++){
        for (int b = 0; b < cols; b++){
            this->mat[a][b] = m.mat[a][b];
        }
    }
}

//Sets all values in the matrix to the value given
template<class T>
void Matrix<T>::operator=(const int i) {
    for (int a = 0; a < this->rows; a++){
        for (int b = 0; b < this->cols; b++){
            this->mat[a][b] = i;
        }
    }
}

//Sets all values in the matrix to the value given
template<class T>
void Matrix<T>::operator=(const double d) {
    for (int a = 0; a < this->rows; a++){
        for (int b = 0; b < this->cols; b++){
            this->mat[a][b] = d;
        }
    }
}

template<class T>
void Matrix<T>::print(const string& caption, int width, int precision) const{
    cout << caption << endl;
    for (int a = 0; a < rows; a++){
        cout << left;
        cout << "| ";
        for (int b = 0; b < cols; b++){
            cout << setw(width) << fixed << setprecision(precision) << mat[a][b] << " ";
        }
        cout << " |" << endl;
    }
}

/**
 * A specialization of the print function that works for uint8_t and char etc.
 * @param caption
 * @param width
 * @param precision
 */
template <>
void Matrix<uint8_t>::print(const string& caption, int width, int precision) const{
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

/**
 * Takes a matrix and extends the pixels around it using the given position. This function will extend the pixels in
 * a line-fashion (i.e. this doesn't do corners). This is just done separately for readability.
 * @param image : The matrix that is getting expanded.
 * @param r : The row of the pixel who's value must be copied.
 * @param c : The col of the pixel who's value must be copied.
 * @param i : The number of cells by which the matrix is being expanded.
 * @param dir : The direction in which the expansion should take place. u = up, d = down, r = right and l = left
 */
 template<class T>
void Matrix<T>::extendLine(Matrix& image, int r, int c, int i, char dir){
    switch (dir) {
        case 'u':
            for (int a = 0; a < i; a++){                //a refers to rows changing
                image.mat[a][c] = image.mat[r][c];
            }
            break;

        case 'd':
            for (int a = r + 1; a < r + i + 1; a++){    //a refers to rows changing
                image.mat[a][c] = image.mat[r][c];
            }
            break;

        case 'l':
            for (int a = 0; a < i; a++){                //a refers to cols changing
                image.mat[r][a] = image.mat[r][c];
            }
            break;

        case 'r':
            for (int a = c + 1; a < c + i + 1; a++){                //a refers to cols changing
                image.mat[r][a] = image.mat[r][c];
            }
            break;

        default:
            string e = "Invalid direction specified.\n";
            throw e;
    }
}

/**
 * Takes a matrix and extends the pixels around it using the given position. This function will extend the pixels in
 * a corner-fashion (i.e. this doesn't do the lines). This is just done separately for readability.
 * @param image : The matrix that is getting expanded.
 * @param r : The row of the pixel who's value must be copied.
 * @param c : The col of the pixel who's value must be copied.
 * @param i : The number of cells by which the matrix is being expanded.
 * @param dir : The direction in which the expansion should take place.
 * a = left top, b = right top, c = left bottom and d = right bottom
 */
 template<class T>
void Matrix<T>::extendCorner(Matrix &image, int r, int c, int i, char dir) {
    int originRow = 0;
    int originCol = 0;
    switch (dir) {
        case 'a':
            originRow = 0;
            originCol = 0;
            break;

        case 'b':
            originRow = 0;
            originCol = c + 1;
            break;

        case 'c':
            originRow = r + 1;
            originCol = 0;
            break;

        case 'd':
            originRow = r + 1;
            originCol = c + 1;
            break;

        default:
            string e = "Invalid direction specified.\n";
            throw e;
    }

    for (int a = originRow; a < originRow + i; a++){
        for (int b = originCol; b < originCol + i; b++){
            image.mat[a][b] = image.mat[r][c];
        }
    }
}

template<class T>
void Matrix<T>::expandMatrix(int i) {
    int newSize = rows + i * 2;
    int newColsSize = cols + i * 2;
    Matrix<T>* biggerMatrix = new Matrix<T>(newSize, newColsSize);  //instantiate new matrix

    int backEdge = newColsSize - i - 1;
    int bottomEdge = newSize -i - 1;

    for (int a = i; a < rows + i; a++){     //fill in the middle of the matrix
        for (int b = i; b < cols + i; b++){
            biggerMatrix->mat[a][b] = mat[a - i][b - i];
        }
    }

    for (int a = i; a < backEdge + 1; a++){ //extend in lines
        Matrix::extendLine(*biggerMatrix, i, a, i, 'u');
        Matrix::extendLine(*biggerMatrix, bottomEdge, a, i, 'd');
    }

    for (int a = i; a < bottomEdge + 1; a++){
        Matrix::extendLine(*biggerMatrix, a, i, i, 'l');
        Matrix::extendLine(*biggerMatrix, a, backEdge, i, 'r');
    }

    Matrix::extendCorner(*biggerMatrix, i, i, i, 'a');  //extend the corners
    Matrix::extendCorner(*biggerMatrix, i, backEdge, i, 'b');
    Matrix::extendCorner(*biggerMatrix, bottomEdge, i, i, 'c');
    Matrix::extendCorner(*biggerMatrix, bottomEdge, backEdge, i, 'd');

//    biggerMatrix->print("Expanded matrix", 6, 2);
    this->rows = biggerMatrix->rows;
    this->cols = biggerMatrix->cols;
    delete this->mat;
    this->mat = biggerMatrix->mat;
}

template<class T>
void Matrix<T>::toArray(T* arr) {
    int i = 0;
    for (int a = 0; a < rows; a++){
        for (int b = 0; b < cols; b++){
            arr[i] = mat[a][b];
            i++;
        }
    }
}

template<class T>
cv::Mat *Matrix<T>::toMat() {
    T pixels[rows * cols];
    this->toArray(pixels);
    auto result = new Mat(rows, cols, CV_8U, pixels);
    return result;
}

template<class T>
Matrix<T>::~Matrix() {
    for (int a = 0; a < rows; a++){
        delete mat[a];
    }
    delete mat;
}

template class Matrix<double>;
template class Matrix<uint8_t>;
template class Matrix<int>;