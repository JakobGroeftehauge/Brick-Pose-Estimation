#include <iostream>
#include <opencv2/opencv.hpp>
//#include "import_json.cpp"

using namespace std;

int main()
{
    vector<cv::Rect> BB;
    cv::FileStorage file("IMG_4040.json", cv::FileStorage::READ);
    cv::FileNode root = file["shapes"];

    for(int i = 0; i < root.size(); i++)
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

    cout << "Hello" << endl;
}
