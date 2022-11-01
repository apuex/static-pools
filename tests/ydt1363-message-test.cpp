#include <apuex/ydt1363.hpp>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace apuex;
using namespace apuex::YDT1363;

bool testEncodeEmptyMessage() {
  bool result = true;
  uint8_t expected[] = { 0x11, 0x01, 0x2a, 0x41, 0x00, 0x00 };
  Message input(0x11, 0x01, 0x2A, 0x41, NULL, 0);
  uint8_t output[6] = {0};
  byte_buffer buf(output, 0, 0, sizeof(output));
  result = input.write(buf);

  cout << "encode Message(0x11, 0x01, 0x2A, 0x41, NULL, 0) => uint8_t[ ";
  for(size_t i = 0; result && i != sizeof(output) && i != sizeof(expected); ++i) {
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

bool testDecodeEmptyMessage() {
  bool result = true;
  uint8_t input[] = { 0x11, 0x01, 0x2a, 0x41, 0x00, 0x00 };
  Message expected(0x11, 0x01, 0x2A, 0x41, NULL, 0);
  Message output;
  byte_buffer buf(input, 0, sizeof(input), sizeof(input));
  result = output.read(buf);
  result = result && (expected == output);

  return result;
}

bool testEncodeMessage() {
  bool result = true;
  uint8_t expected[] = { 0x11, 0x01, 0x2a, 0x41, 0xc0, 0x04, 0xca, 0xfe, 0xbe, 0xef };
  uint8_t info[] = { 0xca, 0xfe, 0xbe, 0xef };
  Message input(0x11, 0x01, 0x2A, 0x41, info, sizeof(info));
  uint8_t output[10] = {0};
  byte_buffer buf(output, 0, 0, sizeof(output));
  result = input.write(buf);

  cout << "encode Message(0x11, 0x01, 0x2A, 0x41, NULL, 0) => uint8_t[ ";
  for(size_t i = 0; result && i != sizeof(output) && i != sizeof(expected); ++i) {
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

bool testDecodeMessage() {
  bool result = true;
  uint8_t input[] = { 0x11, 0x01, 0x2a, 0x41, 0xc0, 0x04, 0xca, 0xfe, 0xbe, 0xef };
  uint8_t info[] = { 0xca, 0xfe, 0xbe, 0xef };
  Message expected(0x11, 0x01, 0x2A, 0x41, info, sizeof(info));
  Message output;
  byte_buffer buf(input, 0, sizeof(input), sizeof(input));
  result = output.read(buf);
  result = result && (expected == output);

  return result;
}

int main(int argc, char* argv[]) {
  return !((testEncodeEmptyMessage() && testDecodeEmptyMessage())
    && (testEncodeMessage() && testDecodeMessage()));
}
