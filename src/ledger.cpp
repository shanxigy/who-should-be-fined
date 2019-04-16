#include "ledger.hpp"

void
Ledger::addRecord(const LedgerRecord& record)
{
  // set new front end
  m_frontEnd = record.getBase64SelfTag();
  // update unsync counters
  m_unSyncRecordCounter++;
  m_unSyncByteCounter += record.m_recordSize;
  // insert the record
  m_ledger.insert(std::pair<std::string, LedgerRecord>(m_frontEnd, record));
}

LedgerRecord
Ledger::getFrontRecord()
{
  return m_ledger[m_frontEnd];
}

void
Ledger::prepareSync(uint8_t* syncPayload)
{
  const LedgerRecord& record = m_ledger[m_frontEnd];
  while (record.getBase64SelfTag() != m_syncedPoint) {
    memcpy(syncPayload, record.m_record, record.m_recordSize);
    record = m_ledger[record.getBase64PreviousRecordTag];
  }
  m_syncedPoint = m_frontEnd;
}