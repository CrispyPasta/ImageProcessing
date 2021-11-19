//
// Created by fouri on 2021/09/30.
//

#ifndef IMAGEPROCESSING_UTILITY_H
#define IMAGEPROCESSING_UTILITY_H
#include "../Matrix/Matrix.h"
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
using namespace std;
using namespace cv;


struct slope{
    double magnitude;
    double direction;
};

struct LabPixel{
    double L;
    double a;
    double b;
};

struct bgrPixel{
    uint8_t b;
    uint8_t g;
    uint8_t r;
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

    static bgrPixel* matToPixels(cv::Mat& image) {
        auto outputArray = new bgrPixel[image.rows * image.cols];

              //make a 2d Array of bgrPixels

        Vec3b temp;

        int total = 0;
        for (int a = 0; a < image.rows; a++) {
            for (int b = 0; b < image.cols; b++) {
                temp = image.at<Vec3b>(a, b);
                outputArray[total] = bgrPixel{temp.val[0], temp.val[1], temp.val[2]};
                total++;
            }
        }

        return outputArray;
    }
};

#endif //IMAGEPROCESSING_UTILITY_H
