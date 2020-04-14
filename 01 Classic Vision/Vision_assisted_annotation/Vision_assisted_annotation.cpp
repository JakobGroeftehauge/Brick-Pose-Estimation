#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

int main()
{
    std::cout << "Hello World!\n";
    std::string folder_path;
    std::string image_list = "";
    std::vector<std::string> image_paths;

    std::ifstream fin;
    std::string word;
    // Open an existing file 
    fin.open(image_list);
    while (std::getline(fin, word))
    {
        //std::cout << word.substr(0, word.find_last_of(".")) + ".json" << std::endl;
        image_paths.push_back(folder_path + "/" + word);
    }
    fin.close();
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
