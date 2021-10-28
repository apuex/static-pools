#include <apuex/modbus.hpp>
#include <stdio.h>

using namespace apuex;
using namespace apuex::modbus;

int main(int argc, char *argv[]) {
  size_t i;
  uint8_t bytes[] = { 0x01, 0x03, 0x04, 0x4B, 0x00, 0x05, 0xF4, 0xEF };
  uint8_t bytes_out[8];
  uint8_t b = 0xff;
  CodecState state;

  struct ModbusRequest input;
  struct ModbusRequest output;
  
  input.address = 0x01;
  input.command = 0x03;
  input.startOfRegister = 0x044b;
  input.length = 0x0005;
  input.crc16 = 0xeff4;

  ModbusResquestParser parser(input);

  printf("[ ");
  bool eq = true;
  for (i = 0, state = Produced; i != sizeof(bytes) && Completed != state && NoContent != state; ++i ) {
    state = parser.encode(bytes_out[i]);
    printf("%02X ", (0xff & bytes_out[i]));
    eq = (eq && (bytes[i] == bytes_out[i]));
  } 
  printf("]\n");

  parser.offer(output);
  for (i = 0, state = Consumed; i != sizeof(bytes) && Completed != state && Rejected != state; ++i ) {
    state = parser.decode(bytes[i]);
  } 

  printf("address         : %02X\n", (0xff & output.address));
  printf("command         : %02X\n", (0xff & output.command));
  printf("startOfRegister : %04X\n", (0xffff & output.startOfRegister));
  printf("length          : %04X\n", (0xffff & output.length));
  printf("crc16           : %04X\n", (0xffff & output.crc16));

  return !(eq && output == input);
}


