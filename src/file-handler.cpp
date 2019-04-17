#include "file-handler.hpp"
#include <sstream>
#include <fstream>

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

  keywordsIt = keywords.begin();
  while (keywordsIt != keywords.end()) {
    keywordsIt->second = keywordsIt->second / totalEntries;
    keywordsIt++;
  }
  return;
}

// Step0: reorder the rest keywords
// Step1: add the popular keywords into the result @forReceiver
// Step2: use row number to determine the rows sent to receiver @receiverID
// Step3: order the selected keywords and append them into the result @forReceiver
void
keyWordsForReceiver(const std::list<uint16_t>& wholeSet, std::list<uint16_t>& forReceiver, int receiverID)
{
  return;
}

void
extractKeyWordsRows(const std::string& fileName, const std::list<uint16_t>& keywords, char* buf)
{
  return;
}

