#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main()
{
    cout << "Hello World!" << endl;

    std::string import_path = "../Strojer_Images/Lower-Res (from c++)/Color";
    //std::string export_path = "../Stroger_Images/Lower-Res (from c++)/Color_RGB/";
    std::string export_path = "/home/jakob/Documents/Bachelor Project/Strojer_Images/Lower-Res (from c++)/Color_RGB/";
    for (const auto & entry : std::experimental::filesystem::directory_iterator(import_path))
    {
        string load_path = entry.path();
        cv::Mat img = imread(load_path, CV_LOAD_IMAGE_COLOR);
        cvtColor(img, img, CV_BGR2RGB);
        vector<int> compression_params;
        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(9);

        cv::imshow("image", img);
        string filename = export_path + entry.path().filename().string();
        imwrite(filename, img, compression_params);
        std::cout << entry.path().filename().string() << std::endl;
        cv::waitKey(0);
    }


    return 0;
}
