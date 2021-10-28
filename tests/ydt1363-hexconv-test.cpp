#include <apuex/ydt1363.hpp>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace apuex;

int main(int argc, char *argv[]) {
  size_t i; // to be compatible with VC6.0
  bool result = true;
  uint8_t halfByte[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15
  };
  uint8_t hexChar[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F'
  };
 
  for(i = 0; i != sizeof(halfByte); ++i) {
    result = result && (halfByte[i] == YDT1363::toHalfByte(hexChar[i]));
    cout << "toHalfByte(): 0x" 
        << hex
        << setw(2) 
        << setfill('0') 
        << uppercase
        << (0xff & YDT1363::toHalfByte(hexChar[i]))
        << ", result = " << result 
        << endl;
  } 

  for(i = 0; i != sizeof(halfByte); ++i) {
    result = result && (hexChar[i] == YDT1363::fromHalfByte(halfByte[i]));
    cout << "fromHalfByte(): 0x" 
        << hex
        << setw(2) 
        << setfill('0') 
        << uppercase
        << (0xff & YDT1363::fromHalfByte(halfByte[i]))
        << ", result = " << result 
        << endl;
  } 

  return !result;
}
