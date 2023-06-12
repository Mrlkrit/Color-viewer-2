#include <regionofinterest.h>




using namespace cv;
using namespace std;





//Mode: 0 means removing black areas, 1 means removing white areas; NeihborMode: 0 means 4 neighborhoods, 1 means 8 neighborhoods;  
void RemoveSmallRegion(Mat& input, Mat& output, int areaSizeLimit, int Mode, int Neihborhoods)
{
    int removeCount = 0; //Number of records removed  
    //record the inspection status of each pixel, 0 - not inspected, 1 - being inspected, 2 - unqualified , 3 - qualified or not required  
    Mat inspectionStatusLabel = Mat::zeros(input.size(), CV_8UC1);

    if (Mode == 1)
    {
        cout << "Remove small areas. ";
        for (int i = 0; i < input.rows; ++i)
        {
            uchar* iData = input.ptr<uchar>(i);
            uchar* iLabel = inspectionStatusLabel.ptr<uchar>(i);
            for (int j = 0; j < input.cols; ++j)
            {
                if (iData[j] < 10)
                {
                    iLabel[j] = 3;
                }
            }
        }
    }
    else
    {
        cout << "Remove holes. ";
        for (int i = 0; i < input.rows; ++i)
        {
            uchar* iData = input.ptr<uchar>(i);
            uchar* iLabel = inspectionStatusLabel.ptr<uchar>(i);
            for (int j = 0; j < input.cols; ++j)
            {
                if (iData[j] > 10)
                {
                    iLabel[j] = 3;
                }
            }
        }
    }

    vector<Point2i> neihborPosition; //Record the position of the neighborhood point  
    neihborPosition.push_back(Point2i(-1, 0));
    neihborPosition.push_back(Point2i(1, 0));
    neihborPosition.push_back(Point2i(0, -1));
    neihborPosition.push_back(Point2i(0, 1));
    if (Neihborhoods == 1)
    {
        cout << "Neighborhoods : 8 neighborhoods." << endl;
        neihborPosition.push_back(Point2i(-1, -1));
        neihborPosition.push_back(Point2i(-1, 1));
        neihborPosition.push_back(Point2i(1, -1));
        neihborPosition.push_back(Point2i(1, 1));
    }
    else cout << "Neighborhoods : 4 neighborhoods." << endl;
    int neihborCount = 4 + 4 * Neihborhoods;
    int x = 0, y = 0;
    //Start testing  
    for (int i = 0; i < input.rows; ++i)
    {
        uchar* iLabel = inspectionStatusLabel.ptr<uchar>(i);
        for (int j = 0; j < input.cols; ++j)
        {
            if (iLabel[j] == 0)
            {
                //********Start the check at that point**********  
                vector<Point2i> pointsGrowthBuffer; //Stack, used to store growth points  
                pointsGrowthBuffer.push_back(Point2i(j, i));
                inspectionStatusLabel.at<uchar>(i, j) = 1;
                int CheckResult = 0; //Used to judge the result (whether it exceeds the size), 0 means not exceeded, 1 means exceeded  

                for (int z = 0; z < pointsGrowthBuffer.size(); z++)
                {

                    for (int q = 0; q < neihborCount; q++) //Check four neighborhood points  
                    {
                        x = pointsGrowthBuffer.at(z).x + neihborPosition.at(q).x;
                        y = pointsGrowthBuffer.at(z).y + neihborPosition.at(q).y;
                        if (x >= 0 && x < input.cols && y >= 0 && y < input.rows) //Prevent out of bounds  
                        {
                            if (inspectionStatusLabel.at<uchar>(y, x) == 0)
                            {
                                pointsGrowthBuffer.push_back(Point2i(x, y)); //Neighborhood points are added to the buffer  
                                inspectionStatusLabel.at<uchar>(y, x) = 1; //Update the check label of the neighborhood point to avoid repeated checks  
                            }
                        }
                    }

                }
                if (pointsGrowthBuffer.size() > areaSizeLimit) CheckResult = 2; //Judgment: whether it exceeds the limited size, 1 is not exceeded, 2 is exceeded  
                else { CheckResult = 1;   removeCount++; }
                for (int z = 0; z < pointsGrowthBuffer.size(); z++) //Update Label record  
                {
                    x = pointsGrowthBuffer.at(z).x;
                    y = pointsGrowthBuffer.at(z).y;
                    inspectionStatusLabel.at<uchar>(y, x) += CheckResult;
                }
                


            }
        }
    }

    Mode = 255 * (1 - Mode);
    //Start to reverse the area that is too small  
    for (int i = 0; i < input.rows; ++i)
    {
        uchar* iData = input.ptr<uchar>(i);
        uchar* iDstData = output.ptr<uchar>(i);
        uchar* iLabel = inspectionStatusLabel.ptr<uchar>(i);
        for (int j = 0; j < input.cols; ++j)
        {
            if (iLabel[j] == 2)
            {
                iDstData[j] = Mode;
            }
            else if (iLabel[j] == 3)
            {
                iDstData[j] = iData[j];
            }
        }
    }

    cout << removeCount << "Number of objects removed." << endl;
}