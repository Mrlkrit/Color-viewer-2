#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace cv;
using namespace std;

Mat Erode(Mat image, int number);
Mat Dilate(Mat image, int number);
Mat Open(Mat image, int number);
Mat Close(Mat image, int number);