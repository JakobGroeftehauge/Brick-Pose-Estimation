#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"
#include "../Includes/annotation_file.h"



int main()
{
    std::cout << "Hello World!\n";
    std::string folder_path = "../../03 Data/Dataset2_onPallet/";
    std::string image_list = "../../03 Data/Dataset2_onPallet/list_of_img_in_OP_train_set_14-04.csv";
    std::vector<std::string> image_names;

    std::ifstream fin;
    std::string word;
    // Open an existing file 
    fin.open(image_list);
    while (std::getline(fin, word))
    {
        image_names.push_back(word);
    }
    fin.close();

    for (int i = 0; i < image_names.size(); i++)
    {
        annotation_file anno_struct;
        cv::Mat image = cv::imread(folder_path + image_names[i]);
        cv::Mat image_org = image.clone();
        double scale = 2.0 / 3.0;
        cv::resize(image, image, cv::Size(), scale, scale, CV_INTER_AREA);
        cv::Rect ROI(0, 0, 960 * scale, 720 * scale);
        int extra_shift = 80;
        cv::Point ROI_shift((image.size().width - ROI.width) / 2 + extra_shift, 0);
        ROI = ROI + ROI_shift;
        image = image(ROI);
        Chamfer_brick_detector chamfer_detector(image);
        chamfer_detector.set_NMS_thresh(120);
        chamfer_detector.set_resolution(90,95,145,15);
        chamfer_detector.detect(image);
        for (int j = 0; j < chamfer_detector.pred_candidates.size(); j++)
        {
            chamfer_detector.pred_candidates[j].rotated_rect.center += cv::Point2f(ROI_shift);
            chamfer_detector.pred_candidates[j].rotated_rect.center *= 1.0 / scale;
            chamfer_detector.pred_candidates[j].rotated_rect.size.width *= 1.0 / scale;
            chamfer_detector.pred_candidates[j].rotated_rect.size.height *= 1.0 / scale;
            std::vector<std::vector<float>> rect;
            cv::Point2f pts[4];
            chamfer_detector.pred_candidates[j].rotated_rect.points(pts);
            for (int k = 0; k < 4; k++)
            {
                rect.push_back({ pts[k].x, pts[k].y });
            }

            for (int k = 0; k < rect.size(); k++)
            {
                std::cout << "size: " << rect[k].size() << " x: " << rect[k][0] << " y: " << rect[k][1] << std::endl;
            }

            anno_struct.polygon_list.push_back(rect);
        }
        anno_struct.file_name = image_names[i];
        anno_struct.relative_path = folder_path;
        anno_struct.img_height = image_org.size().height;
        anno_struct.img_width = image_org.size().width;

        create_annotation_file(anno_struct);

        util::print_rotated_bounding_boxes(image_org, chamfer_detector.pred_candidates);
    }
    return 0;
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
