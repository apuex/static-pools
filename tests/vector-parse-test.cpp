#include <apuex/basic_parser.hpp>
#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>

using namespace std;
using namespace apuex;

bool testEncode() {
  bool result = true;
  size_t i, size;
  std::vector<int32_t> input;
  for(i = 0; i != 5; ++i) {
    input.push_back(i + 1);
  }

  uint8_t expected[] = {
    0x05, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00
  };
  uint8_t output[24] = { 0 };

  BasicVectorParser<int32_t, int32_t> parser(input);
  size = parser.encode(output, sizeof(output));
  cout << "[ ";
  i = 0;
  for(; i != size; ++i) {
    cout 
        << std::hex 
        << std::uppercase
        << std::setw(2) 
        << std::setfill('0')
        << (0xff & output[i])
        << " ";
  }
  cout << "]" << endl;
  cout << std::dec << i << " bytes total." << endl;

  if(sizeof(expected) != size) {
    result = false;
  } else {
    for(i = 0; i != sizeof(expected); ++i) {
      result = result && (expected[i] == output[i]);
    }
  }

  return result;
}

bool testDecode() {
  bool result = true;
  size_t i, size;
  std::vector<int32_t> expected;
  for(i = 0; i != 5; ++i) {
    expected.push_back(i + 1);
  }

  uint8_t input[] = {
    0x05, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00
  };
  std::vector<int32_t> output;

  BasicVectorParser<int32_t, int32_t> parser(output);
  size = parser.decode(input, sizeof(input));

  return (expected == output);
}

int main(int argc, char *argv[]) {
  return !( testEncode()
         && testDecode()
         );
}
