//
// Created by fouri on 2021/09/14.
//

#include "Color.h"
#include <iostream>
#include <math.h>
#include "../Utility/Utility.h"
using namespace std;
using namespace cv;

//* * * * * * * * * CLASSIFICATION COLORS * * * * * * * * *
double red[3] = {48.97, 73.36, 58.26};
double blue[3] = {26.92, 68.81, -91.14};
double green[3] = {87.74, -86.18, 83.18};
double yellow[3] = {97.14, -21.56, 94.48};
double lightBlue[3] = {76.21, -28.32, -30.94};

auto Blue = Matrix<double>(3, 1, blue);
auto Red = Matrix<double>(3, 1, red);
auto Green = Matrix<double>(3, 1, green);
auto Yellow = Matrix<double>(3, 1, yellow);
auto LightBlue = Matrix<double>(3, 1, lightBlue);

double blue20[3] = {48.178, -9.706, -13.369};
double blue40[3] = {46.294, -13.289, -22.190};
double blue60[3] = {34.691, -6.692, -28.208};
double blue80[3] = {28.498, -1.135, -29.853};
double blue100[3] = {23.562, 3.663, -31.702};

auto Blue20 = Matrix<double>(3, 1, blue20);
auto Blue40 = Matrix<double>(3, 1, blue40);
auto Blue60 = Matrix<double>(3, 1, blue60);
auto Blue80 = Matrix<double>(3, 1, blue80);
auto Blue100 = Matrix<double>(3, 1, blue100);

double green20[3] = {53.653, -31.132, 19.784};
double green40[3] = {41.375, -38.189, 23.376};
double green60[3] = {47.798, -44.592, 27.329};
double green80[3] = {44.965, -42.977, 28.115};
double green100[3] = {34.335, -36.918, 22.844};

auto Green20 = Matrix<double>(3, 1, green20);
auto Green40 = Matrix<double>(3, 1, green40);
auto Green60 = Matrix<double>(3, 1, green60);
auto Green80 = Matrix<double>(3, 1, green80);
auto Green100 = Matrix<double>(3, 1, green100);

double red20[3] = {57.287,2.772, -2.277};
double red40[3] = {47.358, 11.836, 1.106};
double red60[3] = {35.950, 30.282, 7.789};
double red80[3] = {28.704, 32.971, 11.669};
double red100[3] = {24.612, 37.998, 14.182};

auto Red20 = Matrix<double>(3, 1, red20);
auto Red40 = Matrix<double>(3, 1, red40);
auto Red60 = Matrix<double>(3, 1, red60);
auto Red80 = Matrix<double>(3, 1, red80);
auto Red100 = Matrix<double>(3, 1, red100);

double yel20[3] = {59.104, -12.389, 19.626};
double yel40[3] = {56.374, -15.439, 42.115};
double yel60[3] = {55.043, -13.892, 54.140};
double yel80[3] = {54.649, -10.138, 54.704};
double yel100[3] = {51.651, -2.168, 54.030};

auto Yel20 = Matrix<double>(3, 1, yel20);
auto Yel40 = Matrix<double>(3, 1, yel40);
auto Yel60 = Matrix<double>(3, 1, yel60);
auto Yel80 = Matrix<double>(3, 1, yel80);
auto Yel100 = Matrix<double>(3, 1, yel100);

double water[3] = {55.822, -2.217, -2.970};
auto Water = Matrix<double>(3, 1, water);


//Matrix<double> classificationColors[] = {Blue, Red, Green, Yellow, LightBlue};
Matrix<double> classificationColors[] = {Blue20, Blue40, Blue60, Blue80, Blue100,
                                         Green20, Green40, Green60, Green80, Green100,
                                         Red20, Red40, Red60, Red80, Red100,
                                         Yel20, Yel40, Yel60, Yel80, Yel100,
                                         Water};

std::string colorNames[] = {"Blue 20%", "Blue 40%", "Blue 60%", "Blue 80%", "Blue 100%",
                            "Green 20%", "Green 40%", "Green 60%", "Green 80%", "Green 100%",
                            "Red 20%", "Red 40%", "Red 60%", "Red 80%", "Red 100%",
                            "Yellow 20%", "Yellow 40%", "Yellow 60%", "Yellow 80%", "Yellow 100%",
                            "Clear/Water"};
int numClassificationColors = 21;
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

Matrix<double> RGBtoXYZ = Matrix<double>(3, 3, RGBtoXYZlist);

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
void Color::rgbtoXyz(Matrix<double>& BGR) {
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
void Color::xyztoLab(Matrix<double>& xyz) {
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
 * This function will take an BGR array (int) and convert it to L*a*b* values. It will create a matrix object and
 * update it until it is L*a*b*. Will decide whether it is desirable to return a matrix or an array later.
 * @param vector : A 3-element integer array containing the RGB value in the order B-G-R.
 */
Matrix<double>* Color::bgrtoLab(int* vector) {
    cout << "Converting from RGB to L*a*b*\n";
    double temp[3];
    temp[0] = vector[2];
    temp[1] = vector[1];
    temp[2] = vector[0];

    auto* lab = new Matrix<double>(3,1, temp);
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
double Color::deltaE(Matrix<double> &val1, Matrix<double> &val2) {
    double Ldif = val1.mat[0][0] - val2.mat[0][0];
    double adif = val1.mat[1][0] - val2.mat[1][0];
    double bdif = val1.mat[2][0] - val2.mat[2][0];
    return sqrt((Ldif * Ldif) + (adif * adif) + (bdif * bdif));
}

string Color::classifyColor(Matrix<double> &val) {
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

    if (verbose) {
        cout << "The minimum delta E value was: " << to_string(minDelta) << endl;
        cout << "The name of the closest match: " << colorNames[minPos] << endl;
    }
    return colorNames[minPos];
}

Matrix<double>*  Color::bgrtoLab(bgrPixel *image, int numPixels) {
    double temp[3];
    Matrix<double>* labPixels = new Matrix<double>[numPixels];


    for (int a = 0; a < numPixels; a++) {
        auto labMat = new Matrix<double>(3, 1);
        labMat->mat[0][0] = image[a].r;
        labMat->mat[1][0] = image[a].g;
        labMat->mat[2][0] = image[a].b;
        rgbtoXyz(*labMat);
        xyztoLab(*labMat);
        labPixels[a] = *labMat;
//        labMat->print();
    }

    return labPixels;
}

Color::~Color() = default;
