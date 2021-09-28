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
string path = "/home/armandt/Desktop/project2021/ImageProcessing/";

bool saveGreyscale(string name, Matrix<uint8_t>& imageMatrix){
    try {
        uint8_t array[imageMatrix.rows * imageMatrix.cols];
        imageMatrix.toArray(array);
        Mat imageMat(imageMatrix.rows, imageMatrix.cols, CV_8U, array);
        imwrite(path + "Images/" + name + ".jpg", imageMat);
        return true;
    } catch (...) {
        cout << "An error occurred while trying to save the image as greyscale.\n";
        return false;
    }
}


bool saveColor(string name, Matrix<uint8_t>& red, Matrix<uint8_t>& green, Matrix<uint8_t>& blue){
    try {
        auto* reconstructedImage = Matrix<uint8_t>::combineChannels(red, green, blue);
        uint8_t array[reconstructedImage->rows * reconstructedImage->cols];
        reconstructedImage->toArray(array);
        Mat imageMat(reconstructedImage->rows, reconstructedImage->cols / 3, CV_8UC3, array);
        imwrite(path + "Images/" + name + ".jpg", imageMat);
        delete reconstructedImage;
        return true;
    } catch (...) {
        cout << "An error occurred while trying to save the image as color.\n";
        return false;
    }
}

void readImage(){
    cout << "\n\n * * * * * IMAGES * * * * * \n";
    try {
        string image_path = path + "Images/snep.jpg";
        Mat snepPicture = imread(image_path, IMREAD_COLOR);

        if (snepPicture.empty()){
            cout << "Image not loaded\n";
            return;
        }
        cout << "Image loaded!\n";
//        cout << snepPicture << "\n\n";
//        imwrite("/home/armandt/Desktop/project2021/ImageProcessing/Images/red.jpg", snepPicture);

        Matrix<uint8_t>* redImage = Edges::getRed(snepPicture);
        Matrix<uint8_t>* blueImage = Edges::getBlue(snepPicture);
        Matrix<uint8_t>* greenImage = Edges::getGreen(snepPicture);
//        redImage->print("Red values", 4, 1);

        cout << "Original rows: " << redImage->rows << endl;
        cout << "Original cols: " << redImage->cols << endl;
        redImage->expandMatrix(3);
        blueImage->expandMatrix(3);
        greenImage->expandMatrix(3);
//        redImage->print("Expanded Red-channel image.");
        cout << "Expanded rows: " << redImage->rows << endl;
        cout << "Expanded cols: " << redImage->cols << endl;

        Edges f(5);
        f.generateGaussian(sqrt(2));
        f.gaussianBlur(*redImage);
        f.gaussianBlur(*blueImage);
        f.gaussianBlur(*greenImage);
//        redImage->print("Blurred image:");
        cout << "Image successfully blurred.\n";

        saveGreyscale("red", *redImage);
        saveGreyscale("blue", *blueImage);
        saveGreyscale("green", *greenImage);
        saveColor("reconst", *redImage, *greenImage, *blueImage);
    } catch (string& e) {
        cout << e;
    }
    cout << "\n * * * * * IMAGES * * * * * \n";
}

void testMatrixFunctions(){
    cout << "\n\n * * * * * MATRIX * * * * * \n";
    try {
        Matrix<int> mat();

        Matrix<int> m1(3, 3, 1);
        Matrix<int> m2(3, 1, 7);
        m1.print("M1", 5);
        m2.print("M2", 5);

        Matrix<int> copied = m2;
        copied.print("Copied from m2", 3);

        Matrix<int> m3 = m1 * m2;
        m3.print("M3 = M1 * M2", 3);

        m3 = m3 + m2;
        m3.print("M3 = M3 + M2", 3);

        Matrix<int> kernel = Matrix<int>(3,3, 1);
        Matrix<int> image2 = Matrix<int>(3, 3, 2);

        Matrix<int> m4 = kernel * image2;
        m4.print("M4 = kernel * image2 ", 3);

        int middleVal = Matrix<int>::convolve(kernel, image2);
        cout << "Result of convolution = " << to_string(middleVal) << endl;

        m4.expandMatrix(2);
        m4.print("M4 was expanded by 2", 3);
    } catch (string& e) {
        cout << e;
    }
    cout << "\n * * * * * MATRIX * * * * * \n";
}

void testColorConversion(){
    cout << "\n\n * * * * * COLOR CONVERSION * * * * * \n";
    try {
       double rgbList[] = {31, 49, 242};
       Matrix<double> RGB(3, 1, rgbList);

       RGB.print("RGB");
       Color::rgbtoXyz(RGB);
       RGB.print("XYZ");
       Color::xyztoLab(RGB);
       RGB.print("LAB");

       int BGR2[] = {242, 85, 17};
       Matrix<double>* RGB2matrix = Color::rgbtoLab(BGR2);

       double difference = Color::deltaE(RGB, *RGB2matrix);
       cout << "Delta E = " << to_string(difference) << endl;

       cout << Color::classifyColor(*RGB2matrix);
    } catch (string& e) {
       cout << e;
    }
    cout << "\n * * * * * COLOR CONVERSION * * * * * \n";
}

void testMixingFunctions(){
    cout << "\n\n * * * * * MIXING * * * * * \n";
    try {
        double col1[3] = {100, 100, 100};       //doesn't really matter that the numbers are unrealistic
        double col2[3] = {75, 75, 75};
        double col3[3] = {50, 50, 50};
        double col4[3] = {25, 25, 25};
        double col5[3] = {0, 0, 0};
 
        Matrix<double> m1(3, 1, col1);
        Matrix<double> m2(3, 1, col2);
        Matrix<double> m3(3, 1, col3);
        Matrix<double> m4(3, 1, col4);
        Matrix<double> m5(3, 1, col5);

        Matrix<double> unmixedPixels[4] = {m1, m1, m5, m5};    //array of Lab pixel values
        Matrix<double> mixedPixels[4] = {m1, m2, m3, m4};
 
        Matrix<double> ave = *(Mixing::averageLab(unmixedPixels, 4));
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
    cout << "\n * * * * * MIXING * * * * * \n";
}

void testEdgeDetection(){
    cout << "\n\n * * * * * EDGES * * * * * \n";
    try{
        Edges e(7);
        e.print("Before");

        e.generateGaussian(sqrt(2));

        e.print("After", 5);

        e.gaussianMatrix.expandMatrix(2);
        e.gaussianMatrix.print("Expanded matrix", 5, 2);

        Matrix<uint8_t> image(15, 15, 1);
        Matrix<uint8_t> kernel(3, 3, 2);
        Matrix<uint8_t> blurredImage(15, 15);

        double d = Matrix<uint8_t>::convolve(kernel, 0, 0, image, blurredImage, 1);
        cout << d << endl;

        Edges f(5);
        f.generateGaussian(sqrt(2));
        Matrix<uint8_t> gaussianStandIn(5, 5, 1);

        f.gaussianBlur(image);
        image.print("Blurred:");
    } catch (string& e){
        cout << e;
    }
    cout << "\n * * * * * EDGES * * * * * \n";
}

int main() {
    readImage();
//    testMatrixFunctions();
//    testColorConversion();
//    testMixingFunctions();
//    testEdgeDetection();
    return 0;
}
