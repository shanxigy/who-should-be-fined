#include "file-handler.hpp"
#include "isaac.hpp"
#include <sstream>
#include <fstream>
#include <random>

std::list<std::pair<uint16_t, double> >
parseKeyWords(const std::string& fileName)
{
  std::list<std::pair<uint16_t, double> > keywords;
  std::ifstream data(fileName);
  std::string delimiter = ",";
  std::string line;
  while(std::getline(data, line)) {
    std::string id = line.substr(0, line.find(delimiter));
    keywords.push_back(std::make_pair(std::stoi(id), 0.0));
  }
  data.close();
  return keywords;
}

void
obtainPopularity(std::list<std::pair<uint16_t, double> >& keywords, const std::list<std::string>& fileNames)
{
  int totalEntries = 0;
  int keywordEntries = 0;
  auto keywordsIt = keywords.begin();
  auto keywordsIt2 = keywords.begin();
  std::string line;

  for (auto fileName : fileNames) {
    std::ifstream data(fileName);
    while(std::getline(data, line)) {
      if (line == std::to_string(keywordsIt->first) + ":") {
        if (keywordsIt2 != keywordsIt) {
          keywordsIt2->second = keywordEntries;
          totalEntries += keywordEntries;
          keywordEntries = 0;
          keywordsIt2++;
        }
        keywordsIt++;
        continue;
      }
      keywordEntries++;
    }
    data.close();
  }

  // insert the results for the last keyword
  keywordsIt2->second = keywordEntries;
  totalEntries += keywordEntries;

  keywordsIt = keywords.begin();
  while (keywordsIt != keywords.end()) {
    keywordsIt->second = keywordsIt->second / totalEntries;
    keywordsIt++;
  }
  std::cout << "Total Entries number: " << totalEntries << std::endl;
  return;
}

std::list<std::pair<uint16_t, double> >
parseHeatMap(const std::string& fileName)
{
  std::list<std::pair<uint16_t, double> > keywords;
  std::ifstream data(fileName);
  std::string delimiter = ",";
  std::string line;
  while(std::getline(data, line)) {
    std::string id = line.substr(0, line.find(delimiter));
    std::string pop = line.substr(line.find(delimiter) + 1);
    keywords.push_back(std::make_pair(std::stoi(id), std::stod(pop)));
  }
  data.close();
  return keywords;
}

std::list<uint16_t>
parseHeatMapOnlyKeys(const std::string& fileName)
{
  std::list<uint16_t> keywords;
  std::ifstream data(fileName);
  std::string delimiter = ",";
  std::string line;
  while(std::getline(data, line)) {
    std::string id = line.substr(0, line.find(delimiter));
    keywords.push_back(std::stoi(id));
  }
  data.close();
  return keywords;
}

void
stripPopularRecords(const std::list<std::pair<uint16_t, double> >& keywords, std::list<uint16_t>& afterStrip,
                    std::list<uint16_t>& popularKeys, double threshold)
{
  afterStrip.clear();
  popularKeys.clear();
  auto keywordsIt = keywords.begin();
  while (keywordsIt != keywords.end()) {
    if (keywordsIt->second > threshold) {
      popularKeys.push_back(keywordsIt->first);
    }
    else {
      afterStrip.push_back(keywordsIt->first);
    }
    keywordsIt++;
  }
  std::cout << "Popular Keywords Size: " << popularKeys.size() << std::endl
  << "Other Keywords Size: " << keywords.size() << std::endl;
}

void
shuffleList(std::list<uint16_t>& keywords)
{
  uint64_t seedVal = 0;
  std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary);
  if (urandom) {
    urandom.read(reinterpret_cast<char*>(&seedVal), sizeof(seedVal));
  }
  else {
    std::cerr << "Cannot read random value from /dev/urandom" << std::endl;
  }

  // shuffle
  std::vector<std::reference_wrapper<const uint16_t> > vec(keywords.begin(), keywords.end());
  isaac64_engine generator(seedVal);
  std::shuffle(vec.begin(), vec.end(), generator);

  // copy the shuffled sequence into a new list
  std::list<uint16_t> shuffled_list {vec.begin(), vec.end()} ;

  // swap the old list with the shuffled list
  keywords.swap(shuffled_list);
}

// Step1: use row number to determine the rows sent to receiver @receiverID
// Step2: order the selected keywords and append them into the result @forReceiver
void
keyWordsForReceiver(const std::list<uint16_t>& wholeSet, std::list<uint16_t>& forReceiver,
                    int receiverID, int maskRange, int field)
{
  int lineNum = 0;
  for (auto it = wholeSet.begin(); it != wholeSet.end(); it++) {
    if (!dropForReceiver(lineNum, receiverID, maskRange, field)) {
      forReceiver.push_back(*it);
    }
    lineNum++;
  }
  forReceiver.sort();
}

void
extractKeyWordsRows(const std::string& fileName, const std::list<uint16_t>& keywords, char* buf)
{
  return;
}

