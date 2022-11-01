#include <apuex/ydt1363.hpp>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace apuex;
using namespace apuex::YDT1363;

bool testEncodeFrame() {
  bool result = true;
  uint8_t expected[] = { 0x7E, 0x31, 0x31, 0x30, 0x31, 0x32, 0x41, 0x34, 0x31, 0x30, 0x30, 0x30, 0x30, 0x46, 0x44, 0x41, 0x35, 0x0D };
  uint8_t input[] = { 0x11, 0x01, 0x2a, 0x41, 0x00, 0x00 };
  uint8_t output[sizeof(expected)] = {0};
  Frame codec;
  CodecState state = Produced;

  codec.offer(input, sizeof(input));

  cout << "encode uint8_t[ 0x11, 0x01, 0x2a, 0x41, 0x00, 0x00 ] => uint8_t[ ";
  for(size_t i = 0; Produced == state && i != sizeof(output); ++i) {
    state = codec.encode(output[i]);
    result = result && (expected[i] == output[i]);
    cout
      << hex
      << setw(2)
      << setfill('0')
      << uppercase
      << (0xff & output[i])
      << " ";
  }
  cout << "]" << endl;
  return result;
}

bool testDecodeFrame() {
  bool result = true;
  uint8_t input[] = { 0x7E, 0x31, 0x31, 0x30, 0x31, 0x32, 0x41, 0x34, 0x31, 0x30, 0x30, 0x30, 0x30, 0x46, 0x44, 0x41, 0x35, 0x0D };
  uint8_t expected[] = { 0x11, 0x01, 0x2a, 0x41, 0x00, 0x00 };
  uint8_t output[sizeof(expected)] = {0};
  Frame codec;

  size_t i = 0;
  CodecState state = Consumed;
  for(i = 0; Consumed == state && i != sizeof(input); ++i) {
    state = codec.decode(input[i]);
  }
  result = result && (Completed == state);
  
  uint16_t len = codec.take(output, sizeof(output));
  result = result && (sizeof(output) == len);

  for(i = 0; result && i != sizeof(output); ++i) {
    result = result && (expected[i] == output[i]);
  }
  return result;
}

int main(int argc, char* argv[]) {
  return !(testEncodeFrame() && testDecodeFrame());
}
