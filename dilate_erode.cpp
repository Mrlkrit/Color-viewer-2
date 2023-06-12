#include <dilate_erode.h>


Mat Erode(Mat image, int number)
{
	/*
	* How it works:
	* 1.Check every pixel for its value
	*
	* 2.If it's white and neighbour is black then change neighbour value to 1
	*   If it's black go to the next pixel
	*
	* 3. Once more check every pixel for it's value
	*   If it's equal to 1 then change it's color to white
	*
	*/
	Mat output;
	image.copyTo(output);
	for (int k = 0; k < number; k++)
	{
		for (int i = 0; i < output.rows; i++) {
			for (int j = 0; j < output.cols; j++) {
				if (output.at<uchar>(i, j) == 255) {
					if (i > 0 && output.at<uchar>(i - 1, j) == 0) output.at<uchar>(i - 1, j) = 1;
					if (j > 0 && output.at<uchar>(i, j - 1) == 0) output.at<uchar>(i, j - 1) = 1;
					if (i + 1 < output.rows && output.at<uchar>(i + 1, j) == 0) output.at<uchar>(i + 1, j) = 1;
					if (j + 1 < output.cols && output.at<uchar>(i, j + 1) == 0) output.at<uchar>(i, j + 1) = 1;
				}
			}
		}
		for (int i = 0; i < output.rows; i++) {
			for (int j = 0; j < output.cols; j++) {
				if (output.at<uchar>(i, j) == 1) {
					output.at<uchar>(i, j) = 255;
				}
			}
		}
	}
	return output;


}


Mat Dilate(Mat image, int number)
{
	/*
	* How it works:
	* 1.Check every pixel for its value
	*
	* 2.If it's black and neighbour is white then change neighbour value to 1
	*   If it's white go to the next pixel
	*
	* 3. Once more check every pixel for it's value
	*   If it's equal to 1 then Paint it Black
	*
	*/
	Mat output;
	image.copyTo(output);
	for (int k = 0; k < number; k++)
	{
		for (int i = 0; i < output.rows; i++) {
			for (int j = 0; j < output.cols; j++) {
				if (output.at<uchar>(i, j) == 0) {
					if (i > 0 && output.at<uchar>(i - 1, j) == 255) output.at<uchar>(i - 1, j) = 1;
					if (j > 0 && output.at<uchar>(i, j - 1) == 255) output.at<uchar>(i, j - 1) = 1;
					if (i + 1 < output.rows && output.at<uchar>(i + 1, j) == 255) output.at<uchar>(i + 1, j) = 1;
					if (j + 1 < output.cols && output.at<uchar>(i, j + 1) == 255) output.at<uchar>(i, j + 1) = 1;
				}
			}
		}
		for (int i = 0; i < output.rows; i++) {
			for (int j = 0; j < output.cols; j++) {
				if (output.at<uchar>(i, j) == 1) {
					output.at<uchar>(i, j) = 0;
				}
			}
		}
	}
	return output;
}

Mat Open(Mat image, int number)
{
	//Open operation is dilate eroded image
	Mat output;
	image.copyTo(output);
	for (int i = 0; i < number; i++)
	{
		output = Erode(output, 1);
		output = Dilate(output, 1);
	}
	return output;
}
Mat Close(Mat image, int number)
{
	//Close operation is erode dilatated image
	Mat output;
	image.copyTo(output);
	for (int i = 0; i < number; i++)
	{
		output = Dilate(output, 1);
		output = Erode(output, 1);
	}
	return output;
}

