#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Matrix/Matrix.h"
#include "Color/Color.h"
#include "Mixing/Mixing.h"
#include "Edges/Edges.h"

using namespace std;
using namespace cv;

bool verbose = false;

void readImage(){
    try {
        string image_path = "/home/armandt/Desktop/project2021/ImageProcessing/Images/tiny.jpg";
        Mat snepPicture = imread(image_path, IMREAD_COLOR);

        if (snepPicture.empty()){
            cout << "Image not loaded\n";
            return;
        }
        cout << "Image loaded!\n";
        cout << snepPicture << "\n\n";
//        imwrite("/home/armandt/Desktop/project2021/ImageProcessing/Images/red.jpg", snepPicture);

        Matrix* redImage = Edges::getRed(snepPicture);
        Matrix* blueImage = Edges::getBlue(snepPicture);
        Matrix* greenImage = Edges::getGreen(snepPicture);
        redImage->print("Red values", 4, 1);

        cout << "Original rows: " << redImage->rows << endl;
        cout << "Original cols: " << redImage->cols << endl;
        redImage->expandMatrix(3);
        redImage->print("Expanded Red-channel image.");
        cout << "Expanded rows: " << redImage->rows << endl;
        cout << "Expanded cols: " << redImage->cols << endl;

        Edges f(5);
        f.generateGaussian(sqrt(2));
        f.gaussianBlur(*redImage);
        redImage->print("Blurred image:");

        double redMatrixArray[redImage->rows * redImage->cols];
        redImage->toArray(redMatrixArray);

        Mat saveImage(redImage->rows, redImage->cols, CV_8U, redMatrixArray);
        cout << saveImage;
        imwrite("/home/armandt/Desktop/project2021/ImageProcessing/Images/red.jpg", saveImage);
        cout << "\nSuccessful?\n";

    } catch (string& e) {
        cout << e;
    }
}

void testMatrixFunctions(){
    try {
        Matrix mat();

        Matrix m1(3, 3, -1);
        Matrix m2(3, 1, 7);
        m1.print("M1", 5);
        m2.print("M2", 5);

        Matrix copied = m2;
        copied.print("Copied from m2");

        Matrix m3 = m1 * m2;
        m3.print("M3 = M1 * M2");

        Matrix kernel = Matrix(3,3, 1);
        Matrix image2 = Matrix(3, 3, 2);

        Matrix m4 = kernel * image2;
        m4.print("M4 = kernel * image2 ");

        double middleVal = Matrix::convolve(kernel, image2);
        cout << "Result of convolution = " << to_string(middleVal) << endl;
    } catch (string& e) {
        cout << e;
    }
}

void testColorConversion(){
    try {
        double rgbList[] = {31, 49, 242};
        Matrix RGB(3, 1, rgbList);

        RGB.print("RGB");
        Color::rgbtoXyz(RGB);
        RGB.print("XYZ");
        Color::xyztoLab(RGB);
        RGB.print("LAB");

        int BGR2[] = {242, 85, 17};
        Matrix* RGB2matrix = Color::rgbtoLab(BGR2);

        double difference = Color::deltaE(RGB, *RGB2matrix);
        cout << "Delta E = " << to_string(difference) << endl;

        cout << Color::classifyColor(*RGB2matrix);
    } catch (string& e) {
        cout << e;
    }
}

void testMixingFunctions(){
    try {
        double col1[3] = {100, 100, 100};       //doesn't really matter that the numbers are unrealistic
        double col2[3] = {75, 75, 75};
        double col3[3] = {50, 50, 50};
        double col4[3] = {25, 25, 25};
        double col5[3] = {0, 0, 0};

        Matrix m1(3, 1, col1);
        Matrix m2(3, 1, col2);
        Matrix m3(3, 1, col3);
        Matrix m4(3, 1, col4);
        Matrix m5(3, 1, col5);


        Matrix unmixedPixels[4] = {m1, m1, m5, m5};    //array of Lab pixel values
        Matrix mixedPixels[4] = {m1, m2, m3, m4};

        Matrix ave = *(Mixing::averageLab(unmixedPixels, 4));
        ave.print("Average L*a*b* value", 3);

        double standardDeviation = Mixing::standardDeviation(unmixedPixels, 4);
        cout << "Standard deviation (unmixed) = " <<  to_string(standardDeviation) << endl;   //for unmixed stuff

        standardDeviation = Mixing::standardDeviation(mixedPixels, 4);
        cout << "Standard deviation (mixed) = " <<  to_string(standardDeviation) << endl;   //for mixed stuff

        double RMI = Mixing::quantifyMixing(mixedPixels, 4, unmixedPixels, 4);
        cout << "RMI for the pixels = " << to_string(RMI) << endl;
    } catch (string& e) {
        cout << e;
    }
}

void testEdgeDetection(){
    try{
        Edges e(7);
        e.print("Before");

        e.generateGaussian(sqrt(2));

        e.print("After", 5);

        e.gaussianMatrix.expandMatrix(2);
        e.gaussianMatrix.print("Expanded matrix", 5, 2);

        Matrix image(15, 15, 1);
        Matrix kernel(3, 3, 2);
        Matrix blurredImage(15, 15);

        double d = Matrix::convolve(kernel, 0, 0, image, blurredImage, 1);
        cout << d << endl;

        Edges f(5);
        f.generateGaussian(sqrt(2));
        Matrix gaussianStandIn(5, 5, 1);

        f.gaussianBlur(image);
        image.print("Blurred:");
    } catch (string& e){
        cout << e;
    }

}

int main() {
    readImage();
//    testMatrixFunctions();
//    testColorConversion();
//    testMixingFunctions();
//    testEdgeDetection();
    return 0;
}
