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

    void print(std::string caption, int s = 5);

    void gaussianBlur(Matrix& image);


    void generateGaussian(double sig);

};


#endif //IMAGEPROCESSING_EDGES_H
