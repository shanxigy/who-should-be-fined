#ifndef LEDGER_LEDGER_H
#define LEDGER_LEDGER_H

#include <iostream>
#include <map>
#include "record.hpp"

typedef uint8_t HashTag[TAG_LENGTH];

class Ledger
{
public:
  void
  addRecord(LedgerRecord);

  LedgerRecord
  getFrontRecord();

public:
  std::map<HashTag, LedgerRecord> m_ledger;
  HashTag m_syncedPoint;
  int m_unSyncRecordCounter;

  // std::list<IP address> m_neighbors;
  // int token;
};

#endif // LEDGER_LEDGER_H
