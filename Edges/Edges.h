//
// Created by fouri on 2021/09/21.
//

#ifndef IMAGEPROCESSING_EDGES_H
#define IMAGEPROCESSING_EDGES_H
#include "../Matrix/Matrix.h"
#include <string>

class Edges {
public:
    double sigma;
    int size;
    int k;
    Matrix gaussianMatrix;

    Edges();

    Edges(int s);

    void generateGaussian(double sig = 1.4142135624);

    void gaussianBlur(Matrix& image);

    void print(std::string caption, int s = 5);

};


#endif //IMAGEPROCESSING_EDGES_H
