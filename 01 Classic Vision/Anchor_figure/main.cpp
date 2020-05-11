#include <iostream>
#include <opencv2/opencv.hpp>

void draw_BB(cv::Mat &img, cv::Point2f center_p, double width, double height, cv::Scalar color = cv::Scalar(0, 0, 255))
{
    cv::RotatedRect rect(center_p, cv::Size2f(width, height), 0);
    cv::Point2f vertices[4];
    rect.points(vertices);

    for(int i = 0; i < 4; i++)
    {
        cv::line(img, vertices[i], vertices[(i + 1) % 4], color, 1, CV_AA);
    }

    cv::circle(img, vertices[1], 3, color, -1,  CV_AA);
    cv::circle(img, vertices[3], 3, color, -1,  CV_AA);
}

void draw_anchors(cv::Mat &img, cv::Point2f center_p, double area, std::vector<std::vector<int>> ratios, std::vector<double> scales ,cv::Scalar color = cv::Scalar(0, 0, 255))
{
    for(int i = 0; i < ratios.size(); i++)
    {
        double wh_scaling = std::sqrt(area/(ratios[i][0] * ratios[i][1]));
        for(int j = 0; j < scales.size(); j++)
        {
            draw_BB(img, center_p, ratios[i][0] * wh_scaling * scales[j], ratios[i][1] * wh_scaling * scales[j], color);
        }
    }
}



using namespace std;

int main()
{
    cv::Mat image;
    image = cv::imread("../../03 Data/Dataset2_onPallet/OP_colorIMG_214.png");
    std::cout << "Cols: " << image.cols <<" Rows: "<< image.rows<< std::endl;
    std::vector<std::vector<int>> ratios = {{1, 2},{1, 1},{2, 1}};
    std::vector<double> scales = {1, std::pow(2, 1.0/3.0), std::pow(2, 2.0/3.0)};

    if(min(image.rows, image.cols) < 800)
    {
        double scaling = 800.0/double(min(image.rows, image.cols));
        std::cout << scaling << endl;
        cv::resize(image, image, cv::Size(image.cols * scaling, image.rows * scaling), 1, 1);

        std::cout << "Rescaled:\n" << "Cols: " << image.cols <<" Rows: "<< image.rows<< std::endl;
    }
    if(max(image.rows, image.cols) > 1333)
    {
        double scaling = 1333.0/double(max(image.rows, image.cols));
        std::cout << scaling << endl;
        cv::resize(image, image, cv::Size(image.cols * scaling, image.rows * scaling), 1, 1);

        std::cout << "Rescaled:\n" << "Cols: " << image.cols <<" Rows: "<< image.rows<< std::endl;
    }

    draw_anchors(image, cv::Point2f(image.cols/2.0, image.rows/2.0), std::pow(512, 2.0), ratios, scales, cv::Scalar(0, 255, 255));
    draw_anchors(image, cv::Point2f(image.cols/4.0 * 3, image.rows/2.0), std::pow(256, 2.0), ratios, scales, cv::Scalar(255, 0, 0));
    draw_anchors(image, cv::Point2f(image.cols/4.0 * 1, image.rows/4.0 * 2.8), std::pow(128, 2.0), ratios, scales, cv::Scalar(0, 0 , 255));
    draw_anchors(image, cv::Point2f(image.cols/4.0 * 1.25, image.rows/4.0 * 1.2), std::pow(64, 2.0), ratios, scales, cv::Scalar(0, 255, 0));
    draw_anchors(image, cv::Point2f(image.cols/4.0 * 0.75, image.rows/4.0 * 1.2), std::pow(32, 2.0), ratios, scales, cv::Scalar(255, 0, 255));


    cv::imshow("test", image);

    imwrite("anchors.png", image);
    cv::waitKey(0);

    cout << "Hello World!" << endl;
    return 0;
}
