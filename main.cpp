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
    cout << "The snep has been summoned.\n";
}

int main() {
    int RGB[] = {251, 165, 45};
    Color::rgbtoxyz(RGB);
    cout << RGB[0] << endl;

    Color c = Color();

    int test[9];
    for (int a = 0; a < 9; a++){
        test[a] = a + 1;
    }

    //    Matrix newconstructor = Matrix(3,3, test);

    cout << Color::linearizeRGB(255) << endl;
    cout << Color::linearizeRGB(41) << endl;
    cout << Color::linearizeRGB(10) << endl;
    return 0;
}
