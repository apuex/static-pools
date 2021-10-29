#include <apuex/ydt1363.hpp>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace apuex;

bool testHexHalfByteConvert() {
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

  return result;
}

bool testHexCharsConvert() {
  size_t i;
  bool result = true;
  uint8_t bytes[] = {
    0xCA, 0xFE, 0xBA, 0xBE, 0xDE, 0xAD, 0xBE, 0xEF
  };
  uint8_t hexChars[] = {
    'C', 'A', 'F', 'E', 'B', 'A', 'B', 'E',
    'D', 'E', 'A', 'D', 'B', 'E', 'E', 'F'
  };
  uint8_t bytesOut[8] = {0};
  uint8_t hexOut[16] = {0};
  result = result 
    && (sizeof(bytes) == YDT1363::fromHexChars(bytesOut, sizeof(bytesOut), hexChars, sizeof(hexChars)));
  for(i = 0; i != sizeof(bytes); ++i) {
    result = result && (bytes[i] == bytesOut[i]);
    cout << "fromHexChars(): 0x" 
        << hex
        << setw(2) 
        << setfill('0') 
        << uppercase
        << (0xff & bytesOut[i])
        << ", result = " << result 
        << endl;
  }
  result = result 
    && (sizeof(hexChars) == YDT1363::toHexChars(hexOut, sizeof(hexOut), bytes, sizeof(bytes)));
  for(i = 0; i != sizeof(hexChars); ++i) {
    result = result && (hexChars[i] == hexOut[i]);
    cout << "toHexChars(): " 
        << hexOut[i]
        << ", result = " << result 
        << endl;
  }

  return result;
}

int main(int argc, char *argv[]) {
  return !( testHexHalfByteConvert()
         && testHexCharsConvert()
         );
}
