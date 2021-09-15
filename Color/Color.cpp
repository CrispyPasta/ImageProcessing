//
// Created by fouri on 2021/09/14.
//

#include "Color.h"
#include <iostream>
using namespace std;
using namespace cv;

//* * * * * * * * * CLASSIFICATION COLORS * * * * * * * * *
double red[3] = {48.97, 73.36, 58.26};
double blue[3] = {30.17, 70.96, -98.40};

auto Blue = Matrix(3, 1, blue);
auto Red = Matrix(3, 1, red);

Matrix classificationColors[] = {Blue, Red};

string colorNames[] = {"Blue", "Red"};
int numClassificationColors = 2;
//* * * * * * * * * CLASSIFICATION COLORS * * * * * * * * *


//RGB to XYZ conversion matrix
double RGBtoXYZlist[9] = {
        0.4124564, 0.3575761, 0.1804375,
        0.2126729, 0.7151522, 0.0721750,
        0.0193339, 0.1191920, 0.9503041
};

//D65 reference while XYZ values
double Xn = 95.04890;
double Yn = 100.0000;
double Zn = 108.8840;

//gamma value for XYZ to LAB function
//double g = 0.2068965517;        // = 6/29
double g2 = 0.1284185493;       // = pow(6/29, 2) * 3
double g3 = 0.008856451679;     // = pow(6/29, 3)

Matrix RGBtoXYZ = Matrix(3, 3, RGBtoXYZlist);

extern bool verbose;

/**
 * The constructor of the Colors class should be called once in order to instantiate the classificationColors
 * array
 */
Color::Color() {
}

/**
 * Converts a three-element RGB array to a three-element double array of the equivalent XYZ values.
 * The given RGB vector will have its values overwritten with the equivalent XYZ values.
 * @param BGR : An integer array of length 3.
 */
void Color::rgbtoXyz(Matrix& BGR) {
    if (verbose){
        cout << "Converting from RGB to XYZ\n";
        cout << "R = " << BGR.mat[0][0] << '\t';
        cout << "G = " << BGR.mat[1][0] << '\t';
        cout << "B = " << BGR.mat[2][0] << '\n';
    }
    
    //linearize the RGB values
    BGR.mat[0][0] = linearizeRGB(BGR.mat[0][0]);
    BGR.mat[1][0] = linearizeRGB(BGR.mat[1][0]);
    BGR.mat[2][0] = linearizeRGB(BGR.mat[2][0]);

    if(verbose)
        BGR.print("Linearized");

    //then do matrix multiplication
    BGR = RGBtoXYZ * BGR;
    BGR *= 100;

    if (verbose)
        BGR.print("RGB to XYZ");
    //this function definitely works (math-wise)
}

/**
 * A small function that was separated from xyztoLab in the interest of readability. It performs
 * an if statement on a given value, and gets used three times for each XYZ to LAB conversion.
 * @param t : fX, fY or fZ
 * @return Double
 */
double xyztoLabStub(double t) {
    if (t > g3){
        return cbrt(t);     //return cube root
    } else {
        return (t / g2) + 0.1379310345;     //constant = 4/29
    }
}

/**
 * Converts XYZ values (in a column vector matrix object) to LAB values. The values in the given matrix
 * will be overwritten.
 * @param xyz : The column vector [X Y Z]T to be converted to LAB.
 */
void Color::xyztoLab(Matrix& xyz) {
    double tmpX = xyz.mat[0][0] / Xn;       //these values will be passed into the conversion function
    double tmpY = xyz.mat[1][0] / Yn;
    double tmpZ = xyz.mat[2][0] / Zn;

    double fX = xyztoLabStub(tmpX);         
    double fY = xyztoLabStub(tmpY);
    double fZ = xyztoLabStub(tmpZ);

    double L = 116.0 * fY - 16.0;
    double a = 500.0 * (fX - fY);
    double b = 200.0 * (fY - fZ);

    xyz.mat[0][0] = L;          //replace the xyz values with the L*a*b* values
    xyz.mat[1][0] = a;
    xyz.mat[2][0] = b;

    if (verbose){
        cout << "Converting from XYZ to L*a*b*\n";
        cout << "X = " <<  xyz.mat[0][0] << '\t';
        cout << "Y = " <<  xyz.mat[1][0] << '\t';
        cout << "Z = " <<  xyz.mat[2][0] << "\n\n";
        cout << "L* = " << L << '\t';
        cout << "a* = " << a << '\t';
        cout << "b* = " << b << '\n';
        xyz.print("L*a*b* value");
    }
}

/**
 * This function will take an RGB array (int) and convert it to L*a*b* values. It will create a matrix object and
 * update it until it is L*a*b*. Will decide whether it is desirable to return a matrix or an array later.
 * @param vector : A 3-element integer array containing the RGB value in the order B-G-R.
 */
Matrix* Color::rgbtoLab(int* vector) {
    cout << "Converting from RGB to L*a*b*\n";

    int temp = vector[0];           //to reverse the order, just swap the first and last elements of the array
    vector[0] = vector[2];
    vector[2] = temp;

    Matrix* lab = new Matrix(3,1, vector);
    rgbtoXyz(*lab);
    xyztoLab(*lab);
    lab->print("Done");
    return lab;
}

/**
 * Linearizes RGB values to a value between 0 and 1.
 * @param C : The R, G or B value to be linearized.
 * @return The linearized double.
 */
double Color::linearizeRGB(int C) {
    double Csrgb = C / 255.0;
    double Clinear = 0.0;

    if (Csrgb <= 0.04045){      //could replace this with C < 11 to save minuscule amount time?
        Clinear = Csrgb / (12.92);
    } else {
        Clinear = pow((Csrgb + 0.055) / 1.055, 2.4);
    }

    return Clinear;
}

/**
 * Calculates the delta E value between two Lab colors and returns it as a double.
 * @param val1 : The first Lab value
 * @param val2 : The second Lab value
 * @return The delta E between val1 and val2
 */
double Color::deltaE(Matrix &val1, Matrix &val2) {
    double Ldif = val1.mat[0][0] - val2.mat[0][0];
    double adif = val1.mat[1][0] - val2.mat[1][0];
    double bdif = val1.mat[2][0] - val2.mat[2][0];
    return sqrt((Ldif * Ldif) + (adif * adif) + (bdif * bdif));
}

string Color::classifyColor(Matrix &val) {
    double minDelta = INFINITY;     //set as biggest possible value
    int minPos = -1;
    double tmpDelta;

    for (int i = 0; i < numClassificationColors; i++){
        tmpDelta = deltaE(val, classificationColors[i]);
        if (tmpDelta < minDelta){
            minDelta = tmpDelta;
            minPos = i;
        }
    }
    verbose = true;
    if (verbose) {
        cout << "The minimum delta E value was: " << to_string(minDelta) << endl;
        cout << "The name of the closest match: " << colorNames[minPos] << endl;
    }
    return colorNames[minPos];
}

Color::~Color() {

}
