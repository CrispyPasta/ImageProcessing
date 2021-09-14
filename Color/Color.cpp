//
// Created by fouri on 2021/09/14.
//

#include "Color.h"
#include <iostream>
using namespace std;
using namespace cv;

Color::Color() {
    RGBtoXYZ.mat[0][0] = 0.4124;
    RGBtoXYZ.mat[0][1] = 0.3576;
    RGBtoXYZ.mat[0][2] = 0.1805;

    RGBtoXYZ.mat[1][0] = 0.2126;
    RGBtoXYZ.mat[1][1] = 0.7152;
    RGBtoXYZ.mat[1][2] = 0.0722;

    RGBtoXYZ.mat[2][0] = 0.0193;
    RGBtoXYZ.mat[2][1] = 0.1192;
    RGBtoXYZ.mat[2][2] = 0.9505;

    RGBtoXYZ.print("RGB to XYZ conversion matrix", 7);
}

void Color::rgbtoxyz(int* RGB) {
    cout << "Converting from RGB to XYZ\n";
    cout << "R = " << RGB[0] << '\t';
    cout << "G = " << RGB[1] << '\t';
    cout << "B = " << RGB[2] << '\n';

}

void Color::xyztoLab() {
    cout << "Converting from XYZ to L*a*b*\n";

}

void Color::rgbtoLab(Mat &image) {
    cout << "Converting from RGB to L*a*b*\n";
}

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