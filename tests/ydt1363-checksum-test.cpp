#include <apuex/ydt1363.hpp>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
using namespace apuex;

int main(int argc, char *argv[]) {
  uint8_t bytes[] = {
    '~', 
    '1', '2', '0', '3', '4', '0', '0', '4',
    '5', '6', 'A', 'B', 'C', 'D', 'F', 'E',
    'F', 'C', '7', '2',
    '\r'
  };

  uint16_t result = YDT1363::checksum((bytes + 1), sizeof(bytes) - 6);

  cout
      << "bytes = '"
      << string(reinterpret_cast<const char*>(bytes), sizeof(bytes))
      << "'"
      << endl
      << "YDT1363::checksum() = 0x"
      << hex
      << setw(4)
      << setfill('0')
      << uppercase 
      << result
      << endl;

  return !(0xFC72 == result);
}
