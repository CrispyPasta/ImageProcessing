#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Matrix/Matrix.h"
#include "Color/Color.h"

using namespace std;
using namespace cv;

void readImage(){
    string image_path = "/home/armandt/Desktop/project2021/ImageProcessing/Images/tiny.jpg";
    Mat snepPicture = imread(image_path, IMREAD_COLOR);

    if (snepPicture.empty()){
        cout << "Oh no the snep was sleeping\n";
        return;
    }
    cout << "The snep has been summoned!\n";
}

void testMatrixFunctions(){
    Matrix<double> mat = Matrix<double>();
    mat.print("Mat");

    Matrix<int> m1 = Matrix<int>(2, 3, -1);
    Matrix<int> m2 = Matrix<int>(3, 2, 7);

    m1.print("M1", 4);
    m2.print("M2", 4);

    try{
        Matrix<int> m3 = m1 * m2;
        m3.print("M3 = M1 * M2");
    } catch (string& s) {
        cout << s << endl;
    }

    Matrix<int> kernel = Matrix<int>(3,3, 1);
    Matrix<int> image2 = Matrix<int>(3, 3, 2);

    try{
        double middleVal = Matrix<int>::convolve(kernel, image2);
        cout << "Result of convolution = " << to_string(middleVal) << endl;
    } catch (string e) {
        cout << e;
    }
}

int main() {
    Color c = Color();

    int RGB[] = {251, 165, 45};
    Color::rgbtoxyz(RGB);
    cout << RGB[0] << endl;

    cout << Color::linearizeRGB(255) << endl;
    cout << Color::linearizeRGB(41) << endl;
    cout << Color::linearizeRGB(10) << endl;

    Color::rgbtoxyz(RGB);
    readImage();

    return 0;
}
