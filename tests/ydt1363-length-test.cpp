#include <apuex/ydt1363.hpp>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace apuex;

bool testToLength(uint16_t input, uint16_t expected) {
  uint16_t result = YDT1363::toLength(input);
  cout
      << "YDT1363::toLength("
      << input
      << ") == 0x" 
      << hex
      << setw(4)
      << setfill('0')
      << uppercase
      << result
      << endl;

  return (expected == result);
}

bool testFromLength(uint16_t input, uint16_t expected) {
  uint16_t result = YDT1363::fromLength(input);
  cout
      << "YDT1363::fromLength(0x"
      << hex
      << setw(4)
      << setfill('0')
      << uppercase
      << input
      << ") == " 
      << dec
      << setw(0)
      << result
      << endl;

  return (expected == result);
}

int main(int argc, char* argv[]) {
  testFromLength(0xD05E, 94);
  return !(testToLength(94, 0xD05E) && testFromLength(0xD05E, 94));
}
