#pragma once
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


double LoG(int x, int y, double sigma);
Mat createMask(int width, int height, double sigma);
Mat padding(Mat img, int width, int height);
Mat convolve(Mat image, Mat pad, Mat kernel, int width, int height);
Mat Gaussian(Mat image, int height, int width, double sigma);