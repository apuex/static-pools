#include "apuex/byte_buffer.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace apuex;

template<typename T>
bool test_number_le(T expected) {
  uint8_t bytes[8];
  // input and expected
  T input = expected;
  // output
  T output = 0;

  byte_buffer write_buffer(bytes, 0, 0, sizeof(bytes));
  write_buffer.writeLittleEndian(input);

  byte_buffer read_buffer(bytes, 0, sizeof(T), sizeof(bytes));
  read_buffer.readLittleEndian(output);
  
  return (input == output);
}

template<typename T>
bool test_number_be(T expected) {
  uint8_t bytes[8];
  // input and expected
  T input = expected;
  // output
  T output = 0;

  byte_buffer write_buffer(bytes, 0, 0, sizeof(bytes));
  write_buffer.writeBigEndian(input);

  byte_buffer read_buffer(bytes, 0, sizeof(T), sizeof(bytes));
  read_buffer.readBigEndian(output);
  
  return (input == output);
}

bool test_little_endian() {
  return (test_number_le(static_cast<uint8_t>(0xca))
    && test_number_le(static_cast<uint16_t>(0xcafe))
    && test_number_le(static_cast<uint32_t>(0xcafebeef))
    && test_number_le(static_cast<uint64_t>(0xcafebeefdeadface))
    && test_number_le(static_cast<float>(3.1415926))
    && test_number_le(static_cast<double>(2.71828))
    );
}

bool test_big_endian() {
  return (test_number_be(static_cast<uint8_t>(0xca))
    && test_number_be(static_cast<uint16_t>(0xcafe))
    && test_number_be(static_cast<uint32_t>(0xcafebeef))
    && test_number_be(static_cast<uint64_t>(0xcafebeefdeadface))
    && test_number_be(static_cast<float>(3.1415926))
    && test_number_be(static_cast<double>(2.71828))
    );
}

int main(int argc, char* argv[]) {

  return !(test_little_endian()
    && test_big_endian());
}

