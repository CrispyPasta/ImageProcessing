#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Matrix/Matrix.h"
#include "Color/Color.h"

using namespace std;
using namespace cv;

bool verbose = false;

void readImage(){
    string image_path = "/home/armandt/Desktop/project2021/ImageProcessing/Images/tiny.jpg";
    Mat snepPicture = imread(image_path, IMREAD_COLOR);

    if (snepPicture.empty()){
        cout << "Image not loaded\n";
        return;
    }
    cout << "Image loaded!\n";
//    cout << snepPicture;
}

void testMatrixFunctions(){
    Matrix mat();

    Matrix m1(3, 3, -1);
    Matrix m2(3, 1, 7);
    m1.print("M1", 4);
    m2.print("M2", 4);

    Matrix copied = m2;
    copied.print("Copied from m2");


    try{
        Matrix m3 = m1 * m2;
        m3.print("M3 = M1 * M2");
    } catch (string& s) {
        cout << s << endl;
    }

    Matrix kernel = Matrix(3,3, 1);
    Matrix image2 = Matrix(3, 3, 2);

    try{
        Matrix m4 = kernel * image2;
        m4.print("M4 = kernel * image2");
    } catch (string & s){
        cout << s << endl;
    }

    try{
        double middleVal = Matrix::convolve(kernel, image2);
        cout << "Result of convolution = " << to_string(middleVal) << endl;
    } catch (string e) {
        cout << e;
    }
}

void testColorConversion(){
    double rgbList[] = {31, 49, 242};
    Matrix RGB(3, 1, rgbList);

    RGB.print("RGB");
    Color::rgbtoXyz(RGB);
    RGB.print("XYZ");
    Color::xyztoLab(RGB);
    RGB.print("LAB");

    int BGR2[] = {242, 85, 17};
    Matrix* RGB2matrix = Color::rgbtoLab(BGR2);

    double difference = Color::deltaE(RGB, *RGB2matrix);
    cout << "Delta E = " << to_string(difference) << endl;

    Color::classifyColor(*RGB2matrix);
}

int main() {
//    readImage();
//    testMatrixFunctions();
    testColorConversion();
    return 0;
}
