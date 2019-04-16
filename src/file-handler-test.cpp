#include "file-handler.hpp"

int
main(int argc, char* argv[])
{
  auto results = parseKeyWords("../../netflix-prize-data/movie_titles.csv");
  std::cout << "result size " << results.size() 
  << "\n the first element " << results.front().first
  << "\n the last element " << results.back().first << std::endl;

  std::list<std::string> fileNames;
  fileNames.push_back("../../netflix-prize-data/combined_data_1.txt");
  // fileNames.push_back("../../netflix-prize-data/combined_data_2.txt");
  // fileNames.push_back("../../netflix-prize-data/combined_data_3.txt");
  // fileNames.push_back("../../netflix-prize-data/combined_data_4.txt");
  obtainPopularity(results, fileNames);

  auto it = results.begin();
  for (int i = 0; i < 100; i++) {
    std::cout << "ID: " << it->first << " Pop :" << it->second << std::endl;
    it++;
  }
  return 0;
}
