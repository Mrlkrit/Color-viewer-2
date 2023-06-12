#include "menu.h"

void DetectOnline()
{

	//kernel 
	int height = 1.0;
	int width = 1.0;
	double sigma = 1.0;

	cout << "Parameters for gaussian noise reduction:" << endl;
	//kernel inputs
	while (height <= 1 || height % 2 == 0 || width <= 1 || width % 2 == 0)
	{
		cout << "Kernel width:" << endl;
		cin >> width;
		cout << "Kernel height:" << endl;
		cin >> height;
		cout << "Sigma:" << endl;
		cin >> sigma;
	}
	waitKey(0);



	namedWindow("Webcam Image Output", WINDOW_NORMAL);
	namedWindow("Webcam Image ", WINDOW_NORMAL);
	namedWindow("HSV Values", WINDOW_AUTOSIZE);
	int Hmin = 0;
	int Hmax = 180;

	int Smin = 0;
	int Smax = 255;

	int Vmin = 0;
	int Vmax = 255;

	createTrackbar("H min", "HSV Values", &Hmin, 180); //Hue (0 - 179)
	createTrackbar("H max", "HSV Values", &Hmax, 180);

	createTrackbar("S min", "HSV Values", &Smin, 255); //Saturation (0 - 255)
	createTrackbar("S max", "HSV Values", &Smax, 255);

	createTrackbar("V min", "HSV Values", &Vmin, 255); //Value (0 - 255)
	createTrackbar("V max", "HSV Values", &Vmax, 255);

	VideoCapture cap(0, CAP_DSHOW);
	if (!cap.isOpened())
	{
		cout << "Error: No video";
		return;
	}

	waitKey(0);
	for (;;)
	{



		Mat frame;
		cap >> frame;
		imshow("Webcam Image ", frame);

		cvtColor(frame, frame, COLOR_BGR2HSV);

		frame = HistogramEqualization(frame);

		frame = Gaussian(frame, height, width, sigma);


		vector<Mat> channels;
		split(frame, channels);
		Mat output = Mat::zeros(frame.size(), CV_8UC1);



		for (int i = 0; i < frame.rows; i++)
		{
			for (int j = 0; j < frame.cols; j++)
			{
				if (channels[0].at<uchar>(i, j) <= Hmax/*Hmax*/ && channels[0].at<uchar>(i, j) >= Hmin/*Hmin*/
					&& channels[1].at<uchar>(i, j) >= Smin/*Smin*/ && channels[1].at<uchar>(i, j) <= Smax/*Smax*/
					&& channels[2].at<uchar>(i, j) >= Vmin /*Vmin*/ && channels[2].at<uchar>(i, j) <= Vmax/*Vmax*/)

					output.at<uchar>(i, j) = 255;

			}
		}
		output = Close(output, 1);

		output = Erode(output, 1);
		output = Dilate(output, 1);

		RemoveSmallRegion(output, output, 60, 1, 1);

		if (waitKey(1) >= 0)
		{
			break;
		}
		imshow("Webcam Image Output", output);
	}
}