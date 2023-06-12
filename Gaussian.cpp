#include <gaussian.h>


double LoG(int x, int y, double sigma)//laplacian of gaussian calculation
{
	double value = 0.0;
	double value1 = 0.0;
	double value2 = 0.0;
	double val = 0.0;

	value = pow(x, 2.0) + pow(y, 2.0) - 2.0 * pow(sigma, 2);
	value1 = 14.0 * pow(sigma, 6.0) / 22.0;
	value2 = -(pow(x, 2.0) + pow(y, 2.0)) / (2.0 * pow(sigma, 2.0));
	val = value * exp(value2) / value1;

	return val;
}

Mat createMask(int width, int height, double sigma)//creating mask
{
	Mat kernel(height, width, CV_64FC1);//prepare matrix

	//distance to kernel image center
	int col = (height - 1) / 2;
	int row = (width - 1) / 2;

	for (int i = -row; i <= row; i++)
	{
		for (int j = -col; j <= col; j++)
		{
			kernel.at<double>(i + row, j + col) = LoG(i, j, sigma);//calculate kernel
		}
	}
	kernel = kernel / sum(kernel);//divide kernel by sum

	
	return kernel;
}

Mat padding(Mat img, int width, int height)//create padding
{
	img.convertTo(img, CV_64FC1);//prepare type
	//distance to kernel image center
	int cols = (width - 1) / 2;
	int rows = (height - 1) / 2;
	//create padding
	Mat pad(Size(img.cols + 2 * cols, img.rows + 2 * rows), CV_64FC1, Scalar(0));
	//copy image to padding
	img.copyTo(pad(Rect(cols, rows, img.cols, img.rows)));
	return pad;
}

//convolution is mathematical operation on two functions to create third function
//in our case we create third image from two
Mat convolve(Mat image, Mat pad, Mat kernel, int width, int height)//convolving
{
	Mat convolve = Mat::zeros(image.size(), CV_64FC1);//prepare matrix
	Mat output;
	kernel.convertTo(kernel, CV_64FC1);//type conversion
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			convolve.at<double>(i, j) = sum(kernel.mul(pad(Rect(j, i, height, width)))).val[0];//convolving padded image and kernel
		}

	}
	convolve.convertTo(output, CV_8UC1);//prepare type for imshow

	return output;
}




Mat Gaussian(Mat image,int height, int width, double sigma)
{
	

	Mat kernel, convolution, LoG, pad;//declare needed matrixes

	//split BGR channels from image
	vector<Mat> channel;
	split(image, channel);//split image channels

	//calculate kernel
	kernel = createMask(width, height, sigma);

	pad = padding(channel[0], width, height);//padding
	convolution = convolve(channel[0], pad, kernel, width, height);//convolution of padding and kernel(Gaussian filter)


	pad = padding(channel[1], width, height);
	convolution = convolve(channel[1], pad, kernel, width, height);


	pad = padding(channel[2], width, height);
	convolution = convolve(channel[2], pad, kernel, width, height);


	merge(channel, LoG);// merging channels

	return LoG;

}


