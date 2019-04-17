#include "file-handler.hpp"
#include <fstream>

int
main(int argc, char* argv[])
{
  // // get the keywords
  // auto results = parseKeyWords("../../netflix-prize-data/movie_titles.csv");
  // std::list<std::string> fileNames;
  // fileNames.push_back("../../netflix-prize-data/combined_data_1.txt");
  // fileNames.push_back("../../netflix-prize-data/combined_data_2.txt");
  // fileNames.push_back("../../netflix-prize-data/combined_data_3.txt");
  // fileNames.push_back("../../netflix-prize-data/combined_data_4.txt");
  // // get the keyword's popularity
  // obtainPopularity(results, fileNames);
  // // output the results to heatmap
  // std::ofstream outputFile;
  // outputFile.open("../heatmap.txt");
  // for (auto it = results.begin(); it != results.end(); it++) {
  //   outputFile << std::fixed << it->first << "," << it->second << std::endl;
  // }
  // outputFile.close();
  auto results = parseHeatMap("../heatmap.txt");

  // strip popular records
  std::list<uint16_t> afterStrip;
  std::list<uint16_t> popKeys;
  stripPopularRecords(results, afterStrip, popKeys);

  // shuffle the unpop records
  shuffleList(afterStrip);

  // output the afterStrip to heatmap
  std::ofstream outputFile;
  outputFile.open("../shuffledListAfterStrip.txt");
  for (auto it = afterStrip.begin(); it != afterStrip.end(); it++) {
    outputFile << std::fixed << *it << std::endl;
  }
  outputFile.close();

  return 0;
}
