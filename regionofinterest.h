#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void RemoveSmallRegion(Mat& input, Mat& output, int areaSizeLimit = 50, int Mode = 1, int Neihbor = 0);