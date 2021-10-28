#ifndef __APUEX_YDT1363_CXX_INCLUDED_
#define __APUEX_YDT1363_CXX_INCLUDED_
#include <apuex/basic_parser.hpp>
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
  std::cout
      << "sum = 0x"
      << std::hex
      << std::setw(4)
      << std::setfill('0')
      << std::uppercase
      << sum
      << std::endl;

  return (1 + (~sum));
}

} /* namespace YDT1363 */

} /* namespace apuex */

#endif /* __APUEX_YDT1363_CXX_INCLUDED_ */

