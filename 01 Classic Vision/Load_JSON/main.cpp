#include <iostream>
#include <opencv2/opencv.hpp>
#include "annotation_loader.h"
#include "data_loader.h"
//#include "import_json.cpp"

void show_BB(cv::Mat img, std::vector<cv::Rect> BB)
{
    for(unsigned int i = 0; i < BB.size(); i++)
    {
        cv::rectangle(img, BB[i].tl(), BB[i].br(), cv::Scalar(255, 0, 0, 1));
    }
}

using namespace std;

int main()
{
    vector<cv::Rect> BB;
    cv::FileStorage file("IMG_4040.json", cv::FileStorage::READ);
    cv::FileNode root = file["shapes"];

    for(unsigned int i = 0; i < root.size(); i++)
    {
        cv::FileNode brick = root[i]["points"];
        //cv::Rect Bounding_box;
        //vector<double>
        cout << "points" << endl;
        for(unsigned int j = 0; j < brick.size(); j++)
        {
              cout << brick[j][0].real() << "   -    " <<  brick[j][1].real()<< endl;
        }
    }

    // Test JSON loader
    Annotation_loader test;
    test.LoadAnnotation("IMG_4040.json");
    cout << test.Rect_list.size();


    //Test image Loader
    Data_loader data_loader("/home/jakob/Documents/Bachelor Project/Load_JSON");
    if(data_loader.Load_Next())
    {
        cout << "found data" << endl;
        cout << data_loader.Bounding_boxes.size() << endl;
    }


    cv::Mat img_BB = data_loader.img.clone();
    show_BB(img_BB, data_loader.Bounding_boxes);
    cv::imshow("Test", img_BB);
    cv::waitKey(0);
    cout << "Hello" << endl;
}
