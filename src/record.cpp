#include "record.hpp"
#include "tinycrypt/tc_sha256.h"
#include "tinycrypt/tc_ecc.h"
#include "tinycrypt/tc_ecc_platform_specific.h"
#include "tinycrypt/tc_ecc_dsa.h"
#include "tinycrypt/tc_constants.h"

/**
   Record Format
   *******************
   * TAG             * // TAG_LENGTH Cover PTAG, UID, PLen, P
   *******************
   * Previous TAG    * // TAG_LENGTH
   *******************
   * UID             * // uint32_t
   *******************
   * Payload Length  * // uint32_t
   *******************
   * Payload         * // Payload Length
   *******************
   * ECDSA           * // ECC_SIG_LENGTH over TAG
   *******************
 */

LedgerRecord::LedgerRecord(const uint8_t* record, uint32_t recordSize)
{
  m_record = (uint8_t*) malloc(recordSize);
  m_recordSize = recordSize;
  memcpy(m_record, record, m_recordSize);
}

LedgerRecord::LedgerRecord(const uint8_t* previousBlock,
                           const uint8_t* payload, uint32_t payloadSize,
                           uint32_t uid,
                           const uint8_t* eccKey, uint32_t eccKeySize)
{
  struct tc_sha256_state_struct s;
  if (tc_sha256_init(&s) != TC_CRYPTO_SUCCESS) {
    std::cout << "Cannot Init TC Sha256" << std::endl;
    return;
  }
  if (tc_sha256_update(&s, previousBlock, TAG_LENGTH) != TC_CRYPTO_SUCCESS) {
    std::cout << "Cannot SHA update: previous TAG" << std::endl;
    return;
  }
  if (tc_sha256_update(&s, (uint8_t*)&uid, sizeof(uint32_t)) != TC_CRYPTO_SUCCESS) {
    std::cout << "Cannot SHA update: UID" << std::endl;
    return;
  }
  if (tc_sha256_update(&s, (uint8_t*)&payloadSize, sizeof(uint32_t)) != TC_CRYPTO_SUCCESS) {
    std::cout << "Cannot SHA update: payload Size" << std::endl;
    return;
  }
  if (tc_sha256_update(&s, payload, payloadSize) != TC_CRYPTO_SUCCESS) {
    std::cout << "Cannot SHA update: payload" << std::endl;
    return;
  }
  uint8_t* tag = (uint8_t*) malloc(TAG_LENGTH);
  if (tc_sha256_final(tag, &s) != TC_CRYPTO_SUCCESS) {
    std::cout << "Cannot SHA final" << std::endl;
    return;
  }
  tc_uECC_set_rng(&default_CSPRNG);
  uint8_t* sig = (uint8_t*) malloc(ECC_SIG_LENGTH);
  if (tc_uECC_sign(eccKey, tag, TAG_LENGTH, sig, tc_uECC_secp256r1()) == 0) {
    std::cout << "Cannot ECC Sign" << std::endl;
    return;
  }

  m_recordSize = TAG_LENGTH * 2 + payloadSize + sizeof(uint32_t) * 2 + ECC_SIG_LENGTH;
  m_record = (uint8_t*) malloc(m_recordSize);
  memcpy(m_record, tag, TAG_LENGTH);
  memcpy(m_record, previousBlock, TAG_LENGTH);
  memcpy(m_record, (uint8_t*)&uid, sizeof(uint32_t));
  memcpy(m_record, (uint8_t*)&payloadSize, sizeof(uint32_t));
  memcpy(m_record, payload, payloadSize);
  memcpy(m_record, sig, ECC_SIG_LENGTH);
}

const uint8_t*
LedgerRecord::getSelfTag() const
{
  return m_record;
}

std::string
LedgerRecord::getBase64SelfTag() const
{
  return base64_encode(m_record, TAG_LENGTH);
}

std::string
LedgerRecord::getBase64PreviousRecordTag() const
{
  return base64_encode(m_record + TAG_LENGTH, TAG_LENGTH);
}

uint32_t
LedgerRecord::getSignerUID() const
{
  int offset = TAG_LENGTH * 2 + sizeof(uint32_t);
  uint32_t ret = *((uint32_t*)(m_record + offset));
  return ret;
}

bool
LedgerRecord::verifySig(const uint8_t* eccKey, uint32_t eccKeySize) const
{
  struct tc_sha256_state_struct s;
  if (tc_sha256_init(&s) != TC_CRYPTO_SUCCESS) {
    std::cout << "Cannot Init TC Sha256" << std::endl;
    return false;
  }
  if (tc_sha256_update(&s, m_record + TAG_LENGTH, m_recordSize - ECC_SIG_LENGTH) != TC_CRYPTO_SUCCESS) {
    std::cout << "Cannot SHA update: PTAG, UID, PLen, P" << std::endl;
    return false;
  }
  uint8_t* tag = (uint8_t*) malloc(TAG_LENGTH);
  if (tc_sha256_final(tag, &s) != TC_CRYPTO_SUCCESS) {
    std::cout << "Cannot SHA final" << std::endl;
    return false;
  }
  if (memcmp(tag, m_record, TAG_LENGTH) != 0) {
    std::cout << "TAG cannot match the record body" << std::endl;
    return false;
  }
  if (tc_uECC_verify(eccKey, tag, TAG_LENGTH, m_record + m_recordSize - ECC_SIG_LENGTH, tc_uECC_secp256r1()) == 0) {
    std::cout << "Verify the ECC signature: failure" << std::endl;
    return false;
  }
  return true;
}

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(uint8_t c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;
}

std::vector<uint8_t>
base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  uint8_t char_array_4[4], char_array_3[3];
  std::vector<uint8_t> ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
          ret.push_back(char_array_3[i]);
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
  }

  return ret;
}