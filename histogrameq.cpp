#include <histogrameq.h>

void process(Mat image, Mat equalizedChannels, int histogram[], float pixelProbability[], float cumulativeDistribution[], int monotonicCDF[])
{
    // Initialize histogram -> set value to 0
    for (int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }

    // For each intensity value calculate number of pixels in image wchich are equal to this value
    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            histogram[(int)image.at<uchar>(y, x)]++;

    // Size of input image 
    int size = image.rows * image.cols;


    //PMF function
    for (int i = 0; i < 256; i++)
    {
        pixelProbability[i] = (double)histogram[i] / size;
    }

    //initialize matrix 
    for (int i = 0; i < 256; i++)
    {
        cumulativeDistribution[i] = 0;
    }
    // Cumulative distribution function
    for (int i = 0; i < 256; i++)
        for (int j = 0; j <= i; j++)
            cumulativeDistribution[i] += pixelProbability[j];

    // Multiply cumulative distribution function with gray level 
    for (int i = 0; i < 256; i++)
        monotonicCDF[i] = cvRound(cumulativeDistribution[i] * 255);


    // Generate the equlized image
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
            equalizedChannels.at<uchar>(i, j) = saturate_cast<uchar>(monotonicCDF[image.at<uchar>(i, j)]);

}
Mat HistogramEqualization(Mat img)
{
    // Prepare histogram which represents number of pixels witch same intensity
    int histogram[256];
    // Prepare matrix to calculate the probability mass function: probability = number of pixels witch same intensity/number of pixels in image
    float pixelProbability[256];
    // Prepare matrix for cumulative distribution function calculation
    float cumulativeDistribution[256];
    //Prepare matrix for monotonic cumulative distribution function 
    int monotonicCDF[256];

    //clone Cody and Rex
    Mat image = img.clone();
    Mat equalizedImg = img.clone();

        vector<Mat> img_channels, equalizedChannels;
        split(image, img_channels);//split BGR channels from image
        equalizedChannels = img_channels;

        // creating histograms of each channel
        process(img_channels[0], equalizedChannels[0], histogram, pixelProbability, cumulativeDistribution, monotonicCDF);

        process(img_channels[1], equalizedChannels[1], histogram, pixelProbability, cumulativeDistribution, monotonicCDF);

        process(img_channels[2], equalizedChannels[2], histogram, pixelProbability, cumulativeDistribution, monotonicCDF);

        merge(equalizedChannels, equalizedImg);// merging channels
        return equalizedImg;
}