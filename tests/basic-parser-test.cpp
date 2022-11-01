#include <apuex/modbus.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace apuex;
using namespace apuex::modbus;

template<typename T>
struct all {
  bool operator()(const T&) const { return true; }
};

int main(int argc, char *argv[]) {
  cout 
        << "sizeof(ModbusRequest) = "
        << sizeof(ModbusRequest)
        << endl;
  size_t i;
  uint8_t ba[] = { 0x01, 0x03, 0x04, 0x4B, 0x00, 0x05, 0xF4, 0xEF };
  uint8_t b = 0xff;
  CodecState state;
  typedef all<uint8_t> all_byte;
  typedef BasicParser<uint8_t, all_byte, false> MyByteParser;
  MyByteParser bp(&b, all_byte());
  state = bp.decode(ba[0x01]);
  printf("state == Accepted => %d\n", (Completed == state));
  printf("b = %02X\n", (0xff & b));

  struct ModbusRequest req;
  struct ModbusRequest req1;
  req.address = 0x01;
  req.command = 0x03;
  req.startOfRegister = 0x044b;
  req.length = 0x0005;
  req.crc16 = 0xf4ef;

  ModbusRequestParser parser(req);

  printf("[ ");
  do {
    state = parser.encode(b);
    printf("%02X ", (0xff & b));
  } while (Completed != state && NoContent!= state);
  printf("]\n");

  parser.offer(req1);
  for (i = 0, state = Consumed; i != sizeof(ba) && Completed != state && Rejected != state; ++i ) {
    state = parser.decode(ba[i]);
  } 
  printf("address         : %02X\n", (0xff & req1.address));
  printf("command         : %02X\n", (0xff & req1.command));
  printf("startOfRegister : %04X\n", (0xffff & req1.startOfRegister));
  printf("length          : %04X\n", (0xffff & req1.length));
  printf("crc16           : %04X\n", (0xffff & req1.crc16));

  return !(req1 == req);
}

