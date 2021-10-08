#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <chrono>
#include "Matrix/Matrix.h"
#include "Color/Color.h"
#include "Mixing/Mixing.h"
#include "Edges/Edges.h"

using namespace std;
using namespace cv;
using namespace chrono;

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

void printEdges(edgePixel** e, int r, int c){
    for (int a = 0; a < r; a++){
        for (int b = 0; b < c; b++){
            cout << "G=" << setw(4) << setprecision(3) << e[a][b].maxMag << '\t';
        }
        cout << endl;
    }
}
void printSlopes(slope** e, int r, int c){
    for (int a = 0; a < r; a++){
        for (int b = 0; b < c; b++){
            cout << "G=" << setw(4) << setprecision(3) << e[a][b].magnitude << '\t';
        }
        cout << endl;
    }
}
// * * * * * * *  * * * * *  * * * * * * private utility functions

void readImage(){
    cout << "\n\n * * * * * IMAGES * * * * * \n";
    try {
        string image_path = path + "Images/tiny.jpg";
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
        redImage->expandMatrix(1);
        blueImage->expandMatrix(1);
        greenImage->expandMatrix(1);
//        redImage->print("Expanded Red-channel image.");
        cout << "Expanded rows: " << redImage->rows << endl;
        cout << "Expanded cols: " << redImage->cols << endl;

        Edges f(3);
        f.generateGaussian(sqrt(2));
        f.gaussianBlur(*redImage);
        f.gaussianBlur(*blueImage);
        f.gaussianBlur(*greenImage);
//        redImage->print("Blurred image:");
        cout << "Image successfully blurred.\n";

//        saveGreyscale("red", *redImage);
//        saveGreyscale("blue", *blueImage);
//        saveGreyscale("green", *greenImage);
        saveColor("rebuilt", *redImage, *greenImage, *blueImage);
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

        double middleVal = Matrix<int>::convolve(kernel, image2);
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
    // try{
    //     Edges e(7, 100);
    //     e.print("Before");

    //     e.generateGaussian(sqrt(2));

    //     e.print("After", 5);

    //     e.gaussianMatrix.expandMatrix(2);
    //     e.gaussianMatrix.print("Expanded matrix", 5, 2);

    //     Matrix<uint8_t> image(15, 15, 1);
    //     Matrix<uint8_t> kernel(3, 3, 2);
    //     Matrix<uint8_t> blurredImage(15, 15);

    //     double d = Matrix<uint8_t>::convolve(kernel, 0, 0, image, blurredImage, 1);
    //     cout << d << endl;

    //     Edges f(5);
    //     f.generateGaussian(sqrt(2));
    //     Matrix<uint8_t> gaussianStandIn(5, 5, 1);

    //     f.gaussianBlur(image);
    //     image.print("Blurred:");

    //     string image_path = path + "Images/tinier.jpg";
    //     Mat snepPicture = imread(image_path, IMREAD_GRAYSCALE);

    //     if (snepPicture.empty()){
    //         cout << "Image not loaded\n";
    //         return;
    //     }
    //     cout << "Image loaded!\n";
    //     cout << snepPicture;

    //     auto sobel = e.sobelPixel(snepPicture, 0, 0);
    //     cout << endl << sobel.magnitude << endl;
    //     cout << sobel.direction << endl;

    //     auto imageMat = Edges::toMatrix(snepPicture);
    //     imageMat->expandMatrix(1);
    //     Mat expandedImage = *(imageMat->toMat());

    //     slope** imageSlopes = new slope*[snepPicture.rows];
    //     for (int a = 0; a < snepPicture.rows; a++){
    //         imageSlopes[a] = new slope[snepPicture.cols];
    //     }

    //     e.sobelImage(expandedImage, imageSlopes, 1);

    //     for (int a = 0; a < snepPicture.rows - 1; a++){
    //         for (int b = 0; b < snepPicture.cols - 1; b++){
    //             cout << "G = " << setprecision(2) << setw(6) << imageSlopes[a][b].magnitude << '\t';
    //             cout << "θ = " << setprecision(2) << setw(6) << imageSlopes[a][b].direction << '\t';
    //         }
    //         cout << endl;
    //     }

    //     edgePixel** edgeTester = e.slopesToEdges(imageSlopes, imageSlopes, imageSlopes, snepPicture.rows, snepPicture.cols);

    //     srand(time(0));
    //     edgePixel testEdgePixel = {11, 3, 13, 2, 12, 1, 'r'};
    //     auto testImage = new edgePixel*[5];
    //     for (int a = 0; a <  5; a++){
    //         testImage[a] = new edgePixel[5];
    //         for (int b = 0; b < 5; b++){
    //             double tmp = rand() % 255;
    //             double tmp1 = rand() % 255;
    //             double tmp2 = rand() % 255;
    //             testEdgePixel.Rmag = tmp;
    //             testEdgePixel.Gmag = tmp1;
    //             testEdgePixel.Bmag = tmp2;
    //             testImage[a][b] = testEdgePixel;
    //         }
    //     }

    //     e.maxMagnitudeGradient(testEdgePixel);
    //     cout << testEdgePixel.maxMag << endl;
    //     cout << testEdgePixel.maxAngle << endl;
    //     e.maxMagnitudeGradient(testImage, 5, 5);
    //     cout << testImage[0][0].maxMag << endl;
    //     cout << testImage[0][0].maxAngle << endl;

    //     e.nonMaximumSuppression(testImage, 5, 5, 1);

    //     Matrix<uint8_t> outputImage(5, 5);

    //     for (int a  = 0; a < 5; a++){
    //         for (int b = 0; b < 5; b++){
    //             e.traceAndThreshold(testImage, outputImage, a, b, 5, 5);
    //         }
    //     }

    //     outputImage.print("Output");

    // } catch (string& e){
    //     cout << e;
    // }
    cout << "\n * * * * * EDGES * * * * * \n";
}

void Canny(){
    try{
        //step 0: prep the gaussian matrix and the lower threshold
        Edges e(5, 10);
        e.generateGaussian();

        //step 1: load the picture
        string image_path = path + "Images/snep.jpg";
        Mat snepPicture = imread(image_path, IMREAD_COLOR);

        if (snepPicture.empty()){
            cout << "Image not loaded\n";
            return;
        }
        cout << "Image loaded!\n";
//        cout << snepPicture;

        //step 2: split it into three channels
        Matrix<uint8_t>* redImage = Edges::getRed(snepPicture);
        Matrix<uint8_t>* blueImage = Edges::getBlue(snepPicture);
        Matrix<uint8_t>* greenImage = Edges::getGreen(snepPicture);

        //step 3: expand the matrices
        cout << "\nOriginal rows: " << redImage->rows << endl;
        cout << "Original cols: " << redImage->cols << endl;
        redImage->expandMatrix(1);
        blueImage->expandMatrix(1);
        greenImage->expandMatrix(1);
        cout << "Expanded rows: " << redImage->rows << endl;
        cout << "Expanded cols: " << redImage->cols << endl;

        //step 4: Gaussian blur all of them
        cout << "Blurring channels.\n";
        e.gaussianBlur(*redImage);
        e.gaussianBlur(*blueImage);
        e.gaussianBlur(*greenImage);
        cout << "Blurring finished.\n";

        //step 5: Apply sobel to all the blurred matrices
        slope** redSlopes = new slope*[snepPicture.rows];
        slope** blueSlopes = new slope*[snepPicture.rows];
        slope** greenSlopes = new slope*[snepPicture.rows];
        for (int a = 0; a < snepPicture.rows; a++){
            redSlopes[a] = new slope[snepPicture.cols];
            blueSlopes[a] = new slope[snepPicture.cols];
            greenSlopes[a] = new slope[snepPicture.cols];
        }

        e.sobelImage(*redImage, redSlopes);
        e.sobelImage(*blueImage, blueSlopes);
        e.sobelImage(*greenImage, greenSlopes);

//        printSlopes(redSlopes, snepPicture.rows, snepPicture.cols);
//        printSlopes(blueSlopes, snepPicture.rows, snepPicture.cols);
//        printSlopes(greenSlopes, snepPicture.rows, snepPicture.cols);
        cout << "Calculated sobel for each channel.\n";

        //step 6: Combine the three channels back into one
        edgePixel** combinedSlopes = e.slopesToEdges(redSlopes, blueSlopes, greenSlopes, snepPicture.rows, snepPicture.cols);
//        printEdges(combinedSlopes, snepPicture.rows, snepPicture.cols);
        cout << "Combined slopes into edgePixel array.\n";

        //step 7: Do nonmaximum suppression
        e.maxMagnitudeGradient(combinedSlopes, snepPicture.rows, snepPicture.cols);
        Utility::print(combinedSlopes, snepPicture.rows, snepPicture.cols);
        cout << endl;
//        e.isLocalMax(combinedSlopes, 1, 1);
        e.nonMaximumSuppression(combinedSlopes, snepPicture.rows, snepPicture.cols);
//        Utility::print(combinedSlopes, snepPicture.rows, snepPicture.cols);
        cout << "Non maximum suppression finished.\n";

        //step 8: trace and threshold
        Matrix<uint8_t> outputBoi(snepPicture.rows, snepPicture.cols, static_cast<char>(0));
        e.traceEdges(combinedSlopes, outputBoi, snepPicture.rows, snepPicture.cols);
        cout << "Tracing and thresholding is done.\n";

        //step 9: save the output
//        outputBoi.print("Output");
        saveGreyscale("edgeOutput", outputBoi);

        cout << "Done.\n";
    } catch (string e) {
        cout << e;
        return;
    }
}

int main() {
    auto start = high_resolution_clock::now();

//    readImage();
//    testMatrixFunctions();
//    testColorConversion();
//    testMixingFunctions();
//    testEdgeDetection();
    Canny();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end-start);
    cout << "Execution complete\n";
    cout << "Time to execute: " << duration.count() << " microseconds.\n";
    return 0;
}
