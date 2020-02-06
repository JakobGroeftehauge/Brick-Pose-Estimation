#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <opencv2/opencv.hpp>


using namespace std;

int main()
{
    cout << "Loading Images..." << endl;

    std::string import_path = "../Strojer_Images/Initial Test Images/Original";
    std::string export_path = "../Strojer_Images/Initial Test Images/Cropped/";

    for (const auto & entry : std::experimental::filesystem::directory_iterator(import_path))
    {
        string load_path = entry.path();
        cv::Mat img = cv::imread(load_path, CV_LOAD_IMAGE_COLOR);

        //Crop Image
        cv::Rect roi;
        int new_width = min(img.size().width, img.size().height)*0.7;
        int new_height = min(img.size().width, img.size().height)*0.7;
        roi.x = (img.size().width - new_width)/2 + 50/2;
        roi.y = (img.size().height - new_height)/2 - 300 + 50/2;
        roi.width = new_width - 50;
        roi.height = new_height - 50;

        img = img(roi);
        cv::resize(img, img, cv::Size(400, 400), 0, 0, cv::INTER_CUBIC);

        cv::imshow("Example", img);

        cv::waitKey(0);
        vector<int> compression_params;
        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(9);

        string filename = export_path + entry.path().filename().string();
        cv::imwrite(filename, img, compression_params);
    }

    std::cout << "Finished!" << endl;
    return 0;
}
