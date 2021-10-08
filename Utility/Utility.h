//
// Created by fouri on 2021/09/30.
//

#ifndef IMAGEPROCESSING_UTILITY_H
#define IMAGEPROCESSING_UTILITY_H
#include "../Matrix/Matrix.h"
#include <iomanip>
using namespace std;


struct slope{
    double magnitude;
    double direction;
};

struct LabPixel{
    double L;
    double a;
    double b;
};

struct edgePixel{
    double Rmag;        //red magnitude
    double Rdir;        //red direction
    double Gmag;
    double Gdir;
    double Bmag;
    double Bdir;
    double maxMag;
    double maxAngle;
};

class Utility{
public:
    static void print(edgePixel e) {
        cout << "R: (" << setprecision(3) << e.Rmag << ", " << e.Rdir << ")";
        cout << "G: (" << setprecision(3) << e.Gmag << ", " << e.Gdir << ")";
        cout << "B: (" << setprecision(3) << e.Bmag << ", " << e.Bdir << ")";
    }

    static void print(edgePixel** e, int r, int c) {
        for (int a = 0; a < r; a++){
            for (int b = 0; b < c; b++){
                cout << setprecision(3) << setw(4) << e[a][b].maxMag << ',' << e[a][b].maxAngle << '\t';
            }
            cout << '\n';
        }
    }
};

#endif //IMAGEPROCESSING_UTILITY_H
