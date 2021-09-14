//
// Created by fouri on 2021/09/14.
//

#include "Color.h"
#include <iostream>
using namespace std;
using namespace cv;

double RGBtoXYZlist[9] = {
        0.4124, 0.3576, 0.1805,
        0.2126, 0.7152, 0.0722,
        0.0193, 0.1192, 0.9505
};

Matrix<double> RGBtoXYZ = Matrix<double>(3, 3, RGBtoXYZlist);

Color::Color() {
//    Color::RGBtoXYZ = new Matrix<double>(3, 3, Color::RGBtoXYZlist);
}

/**
 * Converts a three-element RGB array to a three-element double array of the equivalent XYZ values.
 * @param RGB : An integer array of length 3.
 */
void Color::rgbtoxyz(int* RGB) {
    cout << "Converting from RGB to XYZ\n";
    cout << "R = " << RGB[0] << '\t';
    cout << "G = " << RGB[1] << '\t';
    cout << "B = " << RGB[2] << '\n';

    Matrix<int> RGBmat = Matrix<int>(3, 1, RGB);
    RGBtoXYZ.print("its working??");
}

void Color::xyztoLab() {
    cout << "Converting from XYZ to L*a*b*\n";

}

void Color::rgbtoLab(Mat &image) {
    cout << "Converting from RGB to L*a*b*\n";
}

/**
 * Linearizes RGB values to a value between 0 and 1.
 * @param C : The R, G or B value to be linearized.
 * @return The linearized double.
 */
double Color::linearizeRGB(int C) {
    double Csrgb = C / 256.0;
    double Clinear = 0.0;

    if (Csrgb <= 0.04045){      //could replace this with C < 11 to save minuscule amount time?
        Clinear = Csrgb / (12.92);
    } else {
        Clinear = pow((Csrgb + 0.055) / 1.055, 2.4);
    }

    return Clinear;
}

Color::~Color() {

}