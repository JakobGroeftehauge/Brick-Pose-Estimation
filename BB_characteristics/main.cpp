#include <iostream>
#include "../Includes/data_loader.h"

using namespace std;

int main()
{
    std::ofstream file;
    file.open("results.csv");

    //Do no point to the original annotations, as the Annotation loader will corrupt these annotations.
    std::string path = "/home/jakob/Documents/BachelorProject/Simple Dataset";
    Data_loader loader(path);

   while(loader.loadNext() == true)
   {
       std::vector<std::vector<double>> BB_data = loader.get_BB_characteristics();

       for(unsigned int i = 0; i < BB_data.size(); i++)
       {
           for(unsigned int j = 0; j < BB_data[i].size(); j++)
           {
               if(j == (BB_data[i].size() - 1))
               {
                    file << BB_data[i][j] << "\n";
               }
               else
               {
                    file << BB_data[i][j] << ", ";
               }
           }
       }
    }

    file.close();

    cout << "Hello world" << endl;
    return 1;
}
