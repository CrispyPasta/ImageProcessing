//
// Created by fouri on 2021/09/30.
//

#ifndef IMAGEPROCESSING_UTILITY_H
#define IMAGEPROCESSING_UTILITY_H
#include "../Matrix/Matrix.h"

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
    char max;
    double maxMag;
    double maxAngle;
};

#endif //IMAGEPROCESSING_UTILITY_H
