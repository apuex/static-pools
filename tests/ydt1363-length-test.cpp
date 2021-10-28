#include <apuex/ydt1363.hpp>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace apuex;

int main(int argc, char* argv[]) {
  uint16_t result = YDT1363::length(18);
  cout
      << "YDT1363::length(18) == 0x" 
      << hex
      << setw(4)
      << setfill('0')
      << uppercase
      << result
      << endl;

  return !(0xD012 == result);
}
