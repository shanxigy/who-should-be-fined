#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <iostream>
#include <list>
#include <utility>

static int FIELD = 100; // the value of x in R%x to drop some rows for receiver
static int TOTAL_RECEIVER = 3; // ID: 1, 2, 3
static double LEAST_MAKE_SENSE_PERCENT = 0.75;
static double POPUlAR_KEYWORD_THRESHOLD = 0.0015;

// OT transfer loss rate: 1/5, so 4/5 will be received
static int OT_LOSS_RATE_NUMERATOR = 1;
static int OT_LOSS_RATE_DENOMINATOR = 5;

static int RANGE_SIZE = 0;

static inline void
initRangeSize(int totalRowNumber)
{
  double dropRowSize = (1 - LEAST_MAKE_SENSE_PERCENT * OT_LOSS_RATE_DENOMINATOR / (OT_LOSS_RATE_DENOMINATOR - OT_LOSS_RATE_NUMERATOR)) * totalRowNumber;
  RANGE_SIZE = (int)(dropRowSize * FIELD);
  assert(RANGE_SIZE * TOTAL_RECEIVER < FIELD);
}

static inline bool
dropForReceiver(int totalRowNumber, int rowNumber, int receiverID)
{
  int rangeSize = (1 - LEAST_MAKE_SENSE_PERCENT * OT_LOSS_RATE_DENOMINATOR / (OT_LOSS_RATE_DENOMINATOR - OT_LOSS_RATE_NUMERATOR)) * totalRowNumber;
  return rowNumber % FIELD > (receiverID - 1) * RANGE_SIZE && rowNumber % FIELD < receiverID * RANGE_SIZE;
}

// get the keywords with the popularity
std::list<std::pair<uint16_t, double> >
parseKeyWords(const std::string& fileName);

// scan the whole database to calculate the popularity of each keyword
void
obtainPopularity(std::list<std::pair<uint16_t, double> >& keywords, const std::list<std::string>& fileNames);

// get the keywords with the popularity from heatmap
std::list<std::pair<uint16_t, double> >
parseHeatMap(const std::string& fileName);

// get the keywords only
std::list<uint16_t>
parseHeatMapOnlyKeys(const std::string& fileName);

// return popular keyword list, remaining keywords are in @afterStrip
void
stripPopularRecords(const std::list<std::pair<uint16_t, double> >& keywords, std::list<uint16_t>& afterStrip,
                    std::list<uint16_t>& popularKeys, double threshold = POPUlAR_KEYWORD_THRESHOLD);

// shuffle the list. This fun is cryptographically secured by using ISAAC CSPRNG with /dev/urand seed
void
shuffleList(std::list<uint16_t>& keywords);

// Step0: reorder the rest keywords
// Step1: add the popular keywords into the result @forReceiver
// Step2: use row number to determine the rows sent to receiver @receiverID
// Step3: order the selected keywords and append them into the result @forReceiver
void
keyWordsForReceiver(std::list<std::pair<uint16_t, double> >& wholeSet, std::list<uint16_t>& forReceiver, int receiverID);

// read rows from the dataset into the buf
void
extractKeyWordsRows(const std::string& fileName, const std::list<uint16_t>& keywords, char* buf);


#endif // FILE_HANDLER_H
