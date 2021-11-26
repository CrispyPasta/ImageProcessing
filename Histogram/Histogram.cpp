//
// Created by armandt on 2021/11/23.
//

#include "Histogram.h"

Histogram::Histogram(cv::Mat& input) {
    photo = &input;     //get the address of the image
    historySize = 100;
    histogram = new int[photo->rows];       //size the array appropriately
}

Histogram::~Histogram() {
//    delete photo;
    delete histogram;
}

void Histogram::generateHistogram() {
    for (int a = 0; a < photo->rows; a++){
        int count = 0;
        for (int b = 0; b < photo->cols; b++) {
            int val = photo->at<int>(a, b);
            if (val > 0) {
                count++;
            }
        }
        histogram[a] = count;
//        cout << "Row " << a << ": " << count << '\n';
    }
}

float Histogram::findPeak() {
    int max = -1;
    int maxPos = -1;
    for (int a = 0; a < photo->rows; a++) {
        if (histogram[a] > max) {
            max = histogram[a];
            maxPos = a;
        }
    }

    return 100.0 * (photo->rows - maxPos) / (photo->rows);
}

float Histogram::getFlowRate(int deltaT) {
    this->generateHistogram();
    peakPositions.push_back(this->findPeak());
    peakTimes.push_back(deltaT / 1000000.0);     //assuming the input is in microseconds, convert to seconds

    if (peakPositions.size() > historySize) {
        peakPositions.pop_front();
        peakTimes.pop_front();
    }

    if (peakPositions.size() == 1) {
        return 0;
    } else if (peakPositions.size() > 1) {
        float tmp = peakPositions[peakPositions.size() - 1]  - peakPositions[peakPositions.size() - 2];
        float tmp2 = peakTimes[peakTimes.size() - 1]  - peakTimes[peakTimes.size() - 2];
        return tmp / tmp2;
    }
}
