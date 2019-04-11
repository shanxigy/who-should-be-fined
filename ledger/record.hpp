#ifndef LEDGER_RECORD_H
#define LEDGER_RECORD_H

#include <iostream>
#include <set>
#include <vector>

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

  const uint8_t*
  getSelfTag() const;

  std::string
  getBase64SelfTag() const;

  std::string
  getBase64PreviousRecordTag() const;

  uint32_t
  getSignerUID() const;

  bool
  verifySig(const uint8_t* eccKey, uint32_t eccKeySize) const;

public:
  uint8_t* m_record;
  size_t m_recordSize;
};

std::string
base64_encode(const uint8_t* input, uint32_t len);

std::vector<uint8_t>
base64_decode(std::string const& input);

static inline uint32_t
int32_from_int8(const uint8_t *input)
{
  return (((uint32_t) input[0]) << 24 | ((uint32_t) input[1]) << 16 |
          ((uint32_t) input[2]) <<  8 | ((uint32_t) input[3]) <<  0);
}

#endif // LEDGER_RECORD_H
