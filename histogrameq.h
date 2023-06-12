#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace cv;
using namespace std;

Mat HistogramEqualization(Mat img);
void process(Mat image, Mat equalizedChannels, int histogram[], float pixelProbability[], float cumulativeDistribution[], int monotonicCDF[]);