#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <dilate_erode.h>
#include <histogrameq.h>
#include <gaussian.h>
#include <regionofinterest.h>
using namespace std;
using namespace cv;

void DetectOnline();