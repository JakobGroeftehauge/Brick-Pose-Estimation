// sand_box.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <streambuf>
#include <opencv2/opencv.hpp>
using namespace std;

int main(int argc, char** argv)
{
    cv::Rect A(20, 20, 30, 30); // blue
    cv::Rect B(60, 60, 60, 60); // blue

    cv::Rect C = A & B;   // red


    cv::Mat draw(200, 200, CV_8UC3, cv::Scalar::all(0));
    cv::rectangle(draw, A, cv::Scalar(200, 0, 0), 2);
    cv::rectangle(draw, B, cv::Scalar(200, 0, 0), 2);
    cv::rectangle(draw, C, cv::Scalar(0, 0, 200), 1);
    cv::imshow("test", draw);
    cout << C.area() << endl;
    cv::waitKey(0);
    //string file = "../Strojer_Images/Initial Test Images/Cropped/IMG_4042.JSON";

    //std::ifstream t(file);
    //std::string str((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());

    ////std::string str("null hello name");
    ////string = std::regex_replace(string, std::regex("\\$name"), "Somename");
    //str = std::regex_replace(str, std::regex("null"), "1");
    //cout << str << endl;

    //std::ofstream out(file);
    //out << str;
    //out.close();
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
