#include "file-handler.hpp"
#include <fstream>

int
main(int argc, char* argv[])
{
  auto results = parseKeyWords("../../netflix-prize-data/movie_titles.csv");
  std::cout << "result size " << results.size() 
  << "\n the first element " << results.front().first
  << "\n the last element " << results.back().first << std::endl;

  std::list<std::string> fileNames;
  fileNames.push_back("../../netflix-prize-data/combined_data_1.txt");
  fileNames.push_back("../../netflix-prize-data/combined_data_2.txt");
  fileNames.push_back("../../netflix-prize-data/combined_data_3.txt");
  fileNames.push_back("../../netflix-prize-data/combined_data_4.txt");
  obtainPopularity(results, fileNames);

  auto it = results.begin();
  std::ofstream outputFile;
  outputFile.open("../heatmap.txt");
  for (auto it = results.begin(); it != results.end(); it++) {
    outputFile << std::fixed << it->first << "," << it->second << std::endl;
  }
  outputFile.close();
  return 0;
}
