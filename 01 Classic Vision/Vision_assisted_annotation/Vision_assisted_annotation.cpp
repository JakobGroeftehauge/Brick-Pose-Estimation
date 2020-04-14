#include <iostream>
#include "../Includes/Evaluator.h"
#include "../Includes/Chamfer_brick_detector.h"

int main()
{
    std::cout << "Hello World!\n";


    this->path_folder = path_to_folder;
    this->file_paths_iterator = 0;
    //std::cout << this->path_folder << std::endl;
    std::ifstream fin;
    std::string word;
    // Open an existing file 
    fin.open(img_list_csv);
    while (std::getline(fin, word))
    {
        //std::cout << word.substr(0, word.find_last_of(".")) + ".json" << std::endl;
        this->file_paths.push_back(path_to_folder + "/" + word.substr(0, word.find_last_of(".")) + ".json");
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
