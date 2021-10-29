#ifndef __APUEX_YDT1363_CXX_INCLUDED_
#define __APUEX_YDT1363_CXX_INCLUDED_
#include <apuex/basic_parser.hpp>
#include <exception>
#include <iostream>
#include <iomanip>

namespace apuex {

namespace YDT1363 {

uint16_t length(uint16_t dataLen) {
  uint16_t lengthId = 0xfff & dataLen;
  uint16_t b0 = 0xf & lengthId;
  uint16_t b1 = 0xf & (lengthId >> 4);
  uint16_t b2 = 0xf & (lengthId >> 8);
  uint16_t lchecksum = 0xf & (1 + (~((b0 + b1 + b2) % 16)));

  return 0xffff & ((lchecksum << 12) | lengthId);
}

uint16_t checksum(const uint8_t *bytes, uint16_t len) {
  uint16_t sum = 0;
  for(uint16_t i = 0; i != len; ++i) {
    sum += *(bytes + i);
  }
  return (1 + (~sum));
}

inline uint8_t fromHalfByte(uint8_t c) {
  if(9 >= c) return (c + 0x30);
  else if(0xA <= c && 0xF >= c) return (c + 0x37);
  else throw std::out_of_range("c > 0xF, which is not possible, since input is 4 bits.");
}

inline uint8_t toHalfByte(uint8_t c) {
  if(0x30 <= c && 0x39 >= c) return (c - 0x30);
  else if(0x41 <= c && 0x46 >= c) return (c - 0x37);
  else if(0x61 <= c && 0x66 >= c) return (c - 0x57);
  else throw std::out_of_range("c is Not a valid hex char ([0-9A-Ba-b]).");
}

inline uint16_t fromHexChars(
  uint8_t *bytes, uint16_t bytesLen,
  uint8_t *chars, uint16_t charBuffLen
  ) {
  uint16_t i = 0;
  if(  0 == charBuffLen
    || 0 == bytesLen) return 0;
  for(i = 0; i != charBuffLen && ((i/2) != bytesLen); i += 2) {
    *(bytes + (i/2)) = 0xffff & (
      (toHalfByte(chars[i]) << 4)
      & (toHalfByte(chars[i + 1]))
      );
  }
  return (i / 2);
}

inline uint16_t toHexChars(
  uint8_t *chars, uint16_t charBuffLen, 
  uint8_t *bytes, uint16_t bytesLen
  ) {
  uint16_t i = 0;
  if(  0 == charBuffLen
    || 0 == bytesLen) return 0;
  for(i = 0; i != bytesLen && ((2*i) != charBuffLen); ++i) {
    *(chars + 2*i + 1) = fromHalfByte(0xf & (*(bytes + i)     ));
    *(chars + 2*i    ) = fromHalfByte(0xf & (*(bytes + i) >> 4));
  }
  return (2 * i);
}

} /* namespace YDT1363 */

} /* namespace apuex */

#endif /* __APUEX_YDT1363_CXX_INCLUDED_ */

