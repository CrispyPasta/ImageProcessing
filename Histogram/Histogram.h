//
// Created by armandt on 2021/11/23.
//

#ifndef IMAGEPROCESSING_HISTOGRAM_H
#define IMAGEPROCESSING_HISTOGRAM_H

#include <string>
#include <opencv2/opencv.hpp>
#include <deque>

using namespace std;

class Histogram {
private:
    cv::Mat* photo;
    deque<float> peakPositions;
    deque<float> peakTimes;
    int* histogram;

    int historySize;
public:

    float findPeak();
    explicit Histogram(cv::Mat& photo);

    ~Histogram();

    void generateHistogram();

    float getFlowRate(int deltaT);
};


#endif //IMAGEPROCESSING_HISTOGRAM_H
