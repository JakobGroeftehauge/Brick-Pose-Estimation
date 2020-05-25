#include <iostream>
#include <opencv2/opencv.hpp>
#include "../Includes/annotation_loader.h"


using namespace std;

// OP dataset
string path = "../../03 Data/Dataset2_onPallet/" ;
string file_name =  "OP_colorIMG_185";
cv::Rect roi(420, 80, 800, 600);
int line_thickness = 3;

/*// Simple
string path = "../../03 Data/Simple Dataset Copied/" ;
string file_name =  "colorIMG_159";
cv::Rect roi(230, 100, 400, 320);
int line_thickness = 2;*/

void print_angle_predictions(cv::Mat& img, cv::Rect BB, double angle, int line_width, cv::Scalar color = cv::Scalar(255, 255, 0))
{
    double w = BB.br().x  - BB.tl().x;
    double h = BB.br().y  - BB.tl().y;
    cv::Point2f center = BB.tl() + cv::Point(w/2.0, h/2.0);

    cv::Point2f pt1 = center + cv::Point2f(w/2.0*sin((angle+90)*CV_PI/180)*0.5, -w/2.0*cos((angle+90)*CV_PI/180)*0.5);
    cv::Point2f pt2 = center -  cv::Point2f(w/2.0*sin((angle+90)*CV_PI/180)*0.5, -w/2.0*cos((angle+90)*CV_PI/180)*0.5);

    cv::arrowedLine(img, pt2, pt1, color, line_width, CV_AA, 0, 0.15);
}

void draw_rot_rect(cv::Mat& img, cv::RotatedRect rot_rect, cv::Scalar color, int line_width)
{
    cv::Point2f rect_points[4];
    rot_rect.points(rect_points);

    for (int j = 0; j < 4; j++)
    {
        cv::line(img, rect_points[j], rect_points[(j + 1) % 4], color, line_width, CV_AA);
    }
}


double calculate_IoU(cv::Rect rect1, cv::Rect rect2)
{
    return (double)(rect1 & rect2).area() / (double)(rect1 | rect2).area();
}

void match_annotations(std::vector<std::vector<cv::Point2f>> preds, std::vector<std::vector<cv::Point2f>> GT, std::vector<int> &TP, double IoU_thres)
// Calling the function with 0 uses axis aligned bounding boxes, otherwise rotated are used.
{
    //std::cout << "prediction length: " << detector->predictions.size() << "annotations length: " << loader.annotations.size() << std::endl;
    cv::Mat IoU_mat = cv::Mat::zeros(preds.size(), GT.size(), CV_64F);
    for (int i = 0; i < preds.size(); i++)
    {
        for (int j = 0; j < GT.size(); j++)
        {
                IoU_mat.at<double>(i,j) = calculate_IoU(cv::boundingRect(preds[i]), cv::boundingRect(GT[j]));
        }
    }

    double max;
    int max_idx[2];
    cv::minMaxIdx(IoU_mat, NULL, &max, NULL, max_idx);
    while (max > IoU_thres)
    {
        IoU_mat.row(max_idx[0]) = 0;
        IoU_mat.col(max_idx[1]) = 0;
        TP.push_back(max_idx[0]);
        cv::minMaxIdx(IoU_mat, NULL, &max, NULL, max_idx);
    }
}

std::vector<std::vector<cv::Point2f>> convert_to_point(std::vector<std::vector<double>> coordinates)
{
    std::vector<std::vector<cv::Point2f>> output;
    for(int i = 0; i < coordinates.size();i++)
    {
        output.push_back({cv::Point2f(coordinates[i][0], coordinates[i][1]), cv::Point2f(coordinates[i][2], coordinates[i][3])});
    }

    return output;
}


int main()
{
    cv::Mat img = cv::imread(path + file_name + ".png", cv::IMREAD_COLOR);

    std::vector<std::vector<double>> angles;
    std::vector<std::vector<std::vector<cv::Point2f>>> bboxes;

    /*// Simple dataset
     std::vector<std::vector<double>> retinaNet_vanilla =    {{470.56567, 217.50905, 553.1444 , 312.61038}, {415.5622 , 172.66762, 497.23193, 264.77545}, {392.15277, 161.49098, 476.56122, 253.5651 }, {412.46564, 296.9392 , 490.84253, 394.40118}, {296.0885 , 214.3468 , 371.87387, 310.2319 }, {373.50748, 148.0287 , 456.68732, 239.52895}, {275.70175, 198.84355, 351.6221 , 295.14917}, {345.79102, 124.21866, 433.6658 , 205.07071}, {315.8528 , 228.0665 , 392.1946 , 326.19354}, {435.24518, 189.38522, 514.66113, 284.47235}, {373.26904, 268.14188, 450.2724 , 367.4683 }, {338.97964, 248.71732, 420.42627, 346.93997}, {453.45508, 205.22812, 531.5042 , 299.28668}, {392.29877, 281.77542, 472.91394, 378.19363}};
     bboxes.push_back(convert_to_point(retinaNet_vanilla));

    std::vector<std::vector<double>> retinaNet_w_angle =   {{348.21255, 127.03125, 435.02783, 205.5336 }, {368.90417, 144.16283, 451.73178, 231.16441}, {392.47134, 160.90254, 474.60495, 252.58209}, {471.1256 , 218.18837, 549.9706 , 311.7073 }, {276.6876 , 198.26477, 351.14963, 292.32895}, {296.97244, 213.7592 , 371.94302, 309.1932 }, {414.25488, 172.92407, 494.01077, 265.23572}, {411.79706, 296.03506, 488.4784 , 391.81403}, {435.05252, 189.19017, 514.6747 , 282.1835 }, {316.06973, 227.70213, 392.50452, 324.29398}, {451.9694 , 205.23494, 530.64685, 295.50494}, {375.88205, 268.27365, 450.24805, 365.6899 }, {351.03305, 254.99643, 427.34064, 350.57025}, {331.49133, 244.45288, 408.40366, 339.95325}};
    std::vector<double> retinaNet_w_angle_angles = {-44.463063006111454, -46.58980974382958, -49.27786825772855, -54.397054059518084, -54.4241528350501, -55.04823404970334, -51.592021725893495, -56.508859758426816, -53.48930825844505, -54.799779088454954, -51.893550672087464, -56.35856827592985, -55.78740736696587, -54.8033444472017};
    bboxes.push_back(convert_to_point(retinaNet_w_angle));
    angles.push_back(retinaNet_w_angle_angles);

    std::vector<std::vector<double>> RotinanNet = {{353.6518, 132.58527, 437.53912, 218.12729}, {339.76663, 110.970474, 425.70468, 196.73048}, {276.28333, 196.83514, 349.5252, 292.43378}, {398.42795, 283.8197, 472.52225, 380.1537}, {422.5275, 172.91946, 502.10507, 267.00983}, {333.522, 244.44391, 407.6085, 341.6209}, {315.97882, 225.79596, 388.92633, 323.10516}, {374.77, 148.81738, 457.3632, 239.83752}, {398.97025, 163.7926, 477.6528, 256.0268}, {460.7589, 206.06296, 535.66785, 300.17215}, {479.13916, 221.85355, 557.0292, 314.91647}, {295.68036, 212.60663, 368.7774, 309.3617}, {439.1494, 193.06189, 517.2737, 285.67328}, {418.61884, 297.9972, 492.8476, 395.30475}, {354.49164, 257.61047, 428.53326, 355.31818}, {376.87134, 268.85852, 450.67456, 366.55383}};
    std::vector<double> RotinanNet_angle =  {-45.91988088437265, -44.90274960374185, -57.08363042236714, -56.93260127943982, -52.72588776962897, -57.26363322775365, -58.1844964071277, -49.50036166994171, -52.298953135178984, -55.51137210184898, -53.27306105422785, -57.56338632476342, -52.83878055132553, -57.39241985964883, -57.55556575816375, -57.6405811227629};
    bboxes.push_back(convert_to_point(RotinanNet));
    angles.push_back(RotinanNet_angle);*/

    //OP dataset
    std::vector<std::vector<double>> retinaNet_vanilla =   {{ 876.23456, 290.1904 , 1048.427 , 443.15182}, { 903.8178 , 326.12228, 1074.3202 , 479.86798}, {757.16846, 443.5717 , 931.14355, 602.0866 }, {789.1311 , 479.6314 , 961.5492 , 633.06335}, {788.72815, 191.35896, 960.0582 , 338.37433}, {820.8031 , 226.26987, 989.38824, 373.12677}, {633.49493, 283.84155, 802.9044 , 431.2232 }, { 935.0269 , 361.92313, 1103.7626 , 511.72928}, {734.1972 , 406.0968 , 903.4647 , 555.78253}, {672.63965, 336.8495 , 840.79535, 488.94244}, { 849.5555 , 262.28998, 1018.5437 , 408.53525}, {701.21564, 370.15396, 869.1822 , 521.108 }};
    bboxes.push_back(convert_to_point(retinaNet_vanilla));

    std::vector<std::vector<double>> retinaNet_w_angle =      {{ 905.746 , 328.1237 , 1075.9774 , 478.27213}, {753.8843 , 438.89386, 926.434 , 595.9547 }, {787.81366, 479.88272, 961.2561 , 633.0207 }, {821.8801 , 225.20618, 993.41205, 375.8874 }, {634.22076, 287.3078 , 805.37256, 434.69077}, {789.9662 , 189.73975, 962.74945, 335.4821 }, { 876.1024 , 292.63788, 1045.1107 , 441.57452}, { 934.6257 , 362.71393, 1104.4401 , 509.74744}, {661.54956, 321.2535 , 832.9523 , 472.23294}, {730.8566 , 403.42746, 903.8642 , 553.4951 }, {701.2554 , 370.39706, 872.8457 , 519.6229 }, { 849.9992 , 258.25754, 1020.96136, 408.60165}};
    std::vector<double> retinaNet_w_angle_angles =  {-38.70274100731116, -40.59925231339192, -41.04637698702847, -38.58661071594802, -39.75449793193933, -38.20404157525186, -39.1201202071123, -37.940109405344224, -39.65604758524277, -39.34234041175866, -39.044424635635636, -40.23995362719799};
    bboxes.push_back(convert_to_point(retinaNet_w_angle));
    angles.push_back(retinaNet_w_angle_angles);

    std::vector<std::vector<double>> RotinanNet = {{937.9, 362.19904, 1104.0023, 512.58167}, {649.3561, 300.45325, 813.7477, 448.81903}, {909.0815, 329.40216, 1077.0173, 478.52423}, {789.4961, 189.4573, 964.57153, 332.6011}, {823.5082, 224.0299, 990.5548, 369.55615}, {676.01337, 335.7008, 838.9787, 482.95245}, {851.9574, 257.0205, 1016.43286, 407.8899}, {795.31714, 477.91406, 961.8508, 631.3887}, {622.76196, 262.88702, 790.83203, 414.94836}, {879.4904, 291.06818, 1045.0618, 442.9239}, {760.9314, 440.65363, 926.63306, 596.59906}, {731.1985, 400.7304, 898.30347, 554.53217}, {704.3131, 371.9828, 870.47815, 517.89294}};
    std::vector<double> RotinanNet_angle =  {-40.39099984559824, -40.2205217390093, -39.48199387462333, -35.83536612203728, -38.69199028955756, -40.295602593460664, -40.99107294531888, -41.18454829884324, -40.318695463044484, -40.972819264762016, -42.166138690127866, -41.13272082342118, -38.97791566859264};
    bboxes.push_back(convert_to_point(RotinanNet));
    angles.push_back(RotinanNet_angle);


    std::vector<cv::Mat> images;

    Annotation_loader anno_loader;
    anno_loader.loadAnnotation(path + file_name + ".json");
    std::vector<std::vector<cv::Point2f>> annotations = anno_loader.Rect_list;


    std::vector<int> TP;

    for(int i = 0; i < bboxes.size(); i++)
    {
        cv::Mat temp_img = img.clone();
        TP.clear();
        match_annotations(bboxes[i], annotations, TP, 0.5);

        for(int j = 0; j < bboxes[i].size(); j++)
        {
            if(std::find(TP.begin(), TP.end(), i) != TP.end())
            {
                cv::rectangle(temp_img, cv::boundingRect(bboxes[i][j]).tl(), cv::boundingRect(bboxes[i][j]).br(), cv::Scalar(0, 255, 0), line_thickness);
            }
            else
            {
                 cv::rectangle(temp_img, cv::boundingRect(bboxes[i][j]).tl(), cv::boundingRect(bboxes[i][j]).br(), cv::Scalar(0, 255, 0), line_thickness + 1);
            }
        }

        if(i != 0)
        {
            for(int j = 0; j < angles[i-1].size(); j++)
            {
                if(std::find(TP.begin(), TP.end(), i) != TP.end())
                {
                    print_angle_predictions(temp_img, cv::boundingRect(bboxes[i][j]), angles[i-1][j], line_thickness);

                }
            }
        }

        images.push_back(temp_img);
    }

    cv::imshow(file_name + "_vanilla_retinanet_axis_aligned.png", images[0](roi));
    cv::imshow(file_name + "_retinanet_w_angle_axis_aligned.png", images[1](roi));
    cv::imshow(file_name + "RotinaNet_axis_aligned.png", images[2](roi));

    imwrite(file_name + "_vanilla_retinanet_axis_aligned.png", images[0](roi));
    imwrite(file_name + "_retinanet_w_angle_axis_aligned.png", images[1](roi));
    imwrite(file_name + "RotinaNet_axis_aligned.png", images[2](roi));
    cv::waitKey(0);

    cout << "Hello World!" << endl;
    return 0;
}
