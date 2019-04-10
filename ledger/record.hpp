#ifndef LEDGER_RECORD_H
#define LEDGER_RECORD_H

#include <iostream>
#include <set>

#define TAG_LENGTH 32
#define ECC_SIG_LENGTH 64

/**
   Record Format
   *******************
   * TAG             *
   *******************
   * Previous TAG    *
   *******************
   * UID             *
   *******************
   * Payload Length  *
   *******************
   * Payload         *
   *******************
   * ECDSA           *
   *******************
 */

class LedgerRecord
{
public:
  LedgerRecord(const uint8_t* record, uint32_t recordSize);

  LedgerRecord(const uint8_t* previousBlock,
               const uint8_t* payload, uint32_t payloadSize,
               uint32_t uid,
               const uint8_t* eccKey, uint32_t eccKeySize);

  uint8_t*
  getSelfTag();

  uint32_t
  getSignerUID();

  bool
  verifySig(const uint8_t* eccKey, uint32_t eccKeySize);

public:
  uint8_t* m_record;
  size_t m_recordSize;
};

#endif // LEDGER_RECORD_H
