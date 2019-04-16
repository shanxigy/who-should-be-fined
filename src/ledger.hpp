#ifndef LEDGER_LEDGER_H
#define LEDGER_LEDGER_H

#include <iostream>
#include <map>
#include "record.hpp"

// @TODO: network functionality is missing

class Ledger
{
public:
  void
  addRecord(const LedgerRecord& record);

  LedgerRecord
  getFrontRecord();

  // size should be larger than m_unSyncByteCounter
  void
  prepareSync(uint8_t* syncPayload);

public:
  std::map<std::string, LedgerRecord> m_ledger;
  std::string m_syncedPoint;
  std::string m_frontEnd;
  int m_unSyncRecordCounter;
  int m_unSyncByteCounter;

  // std::list<IP address> m_neighbors;
  // int token;
};

#endif // LEDGER_LEDGER_H
